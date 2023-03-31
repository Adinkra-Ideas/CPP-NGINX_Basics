#include "Request.hpp"

// Constructors
Request::Request() :
	parse_status(FIRST_LINE),
	keep_alive(false)
{
	gettimeofday(&this->start_timer, NULL);
	error_code = NONE;
	this->body = "";
	this->query = "";
}

Request::Request(const Request &copy) :
	parse_status(copy.parse_status),
	buffer(copy.buffer),
	method(copy.method),
	serverName(copy.serverName),
	path(copy.path),
	query(copy.query),
	protocol(copy.protocol),
	request_body(copy.request_body),
	headers(copy.headers),
	chunk_length(copy.chunk_length),
	body_length(copy.body_length),
	length(copy.length),
	start_timer(copy.start_timer),
	last_timer(copy.last_timer),
	error_code(copy.error_code),
	chunk_part(copy.chunk_part),
	body(copy.body),
	keep_alive(copy.keep_alive),
	cgi_exe(copy.cgi_exe),
	cgi_method(copy.cgi_method)
{
}


// Destructor
Request::~Request()
{
}


// Operators
Request & Request::operator=(const Request &assign)
{
		if (this != &assign)
		{
			this->parse_status = assign.parse_status;
			this->buffer = assign.buffer;
			this->method = assign.method;
			this->serverName = assign.serverName;
			this->path = assign.path;
			this->query = assign.query;
			this->protocol = assign.protocol;
			this->request_body = assign.request_body;
			this->headers = assign.headers;
			this->chunk_length = assign.chunk_length;
			this->body_length = assign.body_length;
			this->length = assign.length;
			this->start_timer = assign.start_timer;
			this->last_timer = assign.last_timer;
			this->error_code = assign.error_code;
			this->chunk_part = assign.chunk_part;
			this->body = assign.body;
			this->keep_alive = assign.keep_alive;
			this->cgi_exe = assign.cgi_exe;
			this->cgi_method = assign.cgi_method;
		}
		return *this;
}

	// ****************	CONSTRUCTORS && OPERATORS ENDS	**********************
	//////////////////////////////////////////////////////////////////////////
	// ***********	READING AND WRITING METHODS BEGINS	  ********************

// Returns the content of the _protocol (AKA HTTP Version)
const std::string&	Request::readProtocol( void ) { return protocol; }

// Returns the httpRequest path to doc requested by client
const std::string&	Request::readPath( void ) { return path; }

const std::string&	Request::readQuery( void ) { return this->query; }

// Returns the content of the _method (AKA GET, POST, or DELETE)
const Method&	Request::readMethod( void ) { return method; }

// Returns the status code of the httprequest
const ErrorCode&	Request::readStatusCode( void ) { return error_code; }

void Request::setCgi_exe(std::string str) {this->cgi_exe = str;}
std::string	Request::getCgi_exe() {return this->cgi_exe;}

void Request::setCgi_method(std::string str) {this->cgi_method = str;}
std::string	Request::getCgi_method() {return this->cgi_method;}

std::string Request::getRequestBody()
{
	return (this->body);
}

bool Request::keepAlive()
{
	return (this->keep_alive);
}
	// ************	READING AND WRITING METHODS ENDS	**********************
	//////////////////////////////////////////////////////////////////////////
	// ***********	XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX BEGINS********************

//TODO parsing the request
int Request::parse(std::string &buffer)
{
	size_t error_lvl;
	this->buffer += buffer;
	buffer.clear();
	// parse_status is set to FIRST_LINE as set by request def construc from client.hpp
	// then if first line of client request was parsed successfully, parse_status is set to HEADERS
	if (parse_status == FIRST_LINE)
		error_lvl = first_line();
	if (parse_status == HEADERS)
		error_lvl = parse_headers();
	if (parse_status == PREBODY)
		error_lvl = prepare_for_body();
	if (parse_status == BODY)
		error_lvl = parse_body();
	if (parse_status == CHUNK)			// what does chunk do/mean?
		error_lvl = parse_chunks();
	return (error_lvl);
}

//TODO also check total length of the request
int Request::first_line()
{
	//TODO more checks if line is ok
	if (this->buffer.find(EOL) != std::string::npos)   // EOL == '\n'
	{
		size_t start = 0;
		size_t end = this->buffer.find_first_of(" ", start);
		parseMethod(this->buffer.substr(start, end)); //here
		if (this->error_code)
			return (this->error_code);
		start = end + 1;
		end = this->buffer.find_first_of(" ", start);
		parsePath(this->buffer.substr(start, end - start));
		if (this->error_code)
			return (this->error_code);
		start = end + 1;
		end = this->buffer.find_first_of(EOL, start);
		parseProtocol(this->buffer.substr(start, end - start));
		if (this->error_code)
			return (this->error_code);
		this->buffer.erase(0, end + 2);
		this->parse_status = HEADERS;
		return (this->error_code);
	}
	else
	{
		this->parse_status = COMPLETED;
		this->error_code = BADREQUEST;
		return (this->error_code);
	}
	
	
}
void Request::parseProtocol(std::string str)
{
	if (!str.compare("HTTP/1.1"))
		this->protocol = "HTTP/1.1";
	else
		this->error_code = BADREQUEST;
}

void Request::parseMethod(std::string str)
{
	if (!str.compare("GET"))
		this->method = GET;
	else if (!str.compare("POST"))
		this->method = POST;
	else if (!str.compare("DELETE"))
		this->method = DELETE;
	else 
		this->error_code = BADREQUEST;
}

void Request::parsePath(std::string str)
{
	if (str.at(0) != '/')
		this->error_code = BADREQUEST;
	else
	{
		size_t index = str.find("?");
		if( index == std::string::npos)
			this->path = str;
		else
		{
			this->path = str.substr(0, index);
			this->query = str.substr(index + 1);
		}
		if (not_allowed_char_in_URL())
		{
			std::cout << "url erro" << std::endl;
			this->error_code = BADREQUEST;
		}
	}
}

int Request::parse_headers()
{
	//TODO parse header for atleast Host
	//TODO extra stuff to parse for: Transfer-Encoding, Content-Length, Connection
	size_t start = 0;
	size_t end = this->buffer.find_first_of(EOL, start);
	size_t delimiter;
	std::string key;
	std::string value;
	// copy all the lines below line one from request Header
	// to a map of Name=>Value
	while (end != std::string::npos)
	{
		if(this->buffer.find_first_of(EOL, start) == start)
			break ;
		delimiter = this->buffer.find_first_of(':', start);
		key = this->buffer.substr(start, delimiter - start);
		//TODO value might have leading whitespace and trailing or not
		value = this->buffer.substr(delimiter + 2, end - delimiter - 2);
		if (not_allowed_char_in_field(value))
		{
			std::cout << "field erro:" << value << std::endl;
			this->parse_status = COMPLETED;
			this->error_code = BADREQUEST;
			return 1;

		}
		this->headers[http::to_lower_case(key)] = value;				// we need to print out what'S stored in headers map object
		start = end + 2;
		end = this->buffer.find_first_of(EOL, start);
	}
	// std::cout << "\n\nmap header holds: \n";
	// for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it )
	// 	std::cout << "Key =" << (it->first) << "$ value =" << (it->second) << "$" <<std::endl;

	buffer.erase(0, end + 2);
	this->parse_status = PREBODY;
	return (this->error_code);
}



int Request::prepare_for_body()
{
	//TODO check if header is ok
	//TODO prepare chunk receiving
	if (this->headers.find("host") == this->headers.end() || this->headers["host"].empty())
	{
		this->error_code = BADREQUEST;
		this->parse_status = COMPLETED;
		return (this->error_code);
	}
	std::map<std::string, std::string>::iterator iter = this->headers.find("connection");
	if (iter != this->headers.end())
	{
		if (iter->second == "keep-alive")
		{
			this->keep_alive = true;
		}
	}
	if(this->headers.find("transfer-encoding") != this->headers.end() && this->headers["transfer-encoding"] == "chunked")
	{
		this->parse_status = CHUNK;
		this->chunk_part = CHUNKSIZE;
	}
	else if (this->headers.find("content-length") != this->headers.end())
	{
		try
		{
			this->body_length = parse_str_to_int(this->headers["content-length"]);	
		}
		catch(const std::exception& e)
		{
			this->error_code = BADREQUEST;
			this->parse_status = COMPLETED;
			return (this->error_code);
		}
		this->parse_status = BODY;
	}
	else
	{
		this->parse_status = COMPLETED;
	}
	return (this->error_code);
	if (this->headers.find("Host") == this->headers.end() || this->headers["Host"].empty())
	{
		this->error_code = BADREQUEST;
		this->parse_status = COMPLETED;
	}
	else
	{
		this->parse_status = COMPLETED;
	}
	return (this->error_code);
}

size_t Request::parse_str_to_int(std::string str)
{
	long result = 0;
	int str_len = str.length();
	for (int i = 0; i < str_len; ++i)
	{
		if(isdigit(str[i]))
		{
			result *= 10;
			result += str[i] - '0';
		}
		else
			throw ;
		if (result > MAXBODYSIZE)
			throw ;
	}
	return (result);
}

int Request::parse_body()
{
	this->body += this->buffer;
	this->buffer.clear();
	if (this->body.length() == this->body_length)
	{
		this->parse_status = COMPLETED;
	}
	else if (this->body.length() > this->body_length)
	{
		this->parse_status = COMPLETED;
		this->error_code = 	BADREQUEST;
	}
	return (this->error_code);
}

int Request::parse_chunks()
{
	std::cout << "chunk parsing" << std::endl;
	this->parse_status = COMPLETED;
	return (0);
}

bool Request::parsingFinished()				// I dont fully understand the idea behind this
{
	return (this->parse_status == COMPLETED);
}

std::string Request::getServerName()
{
	return (this->serverName);
}

ErrorCode Request::getErrorCode()
{
	return (this->error_code);
}

void Request::clear()
{
	this->error_code = NONE;
	this->parse_status = FIRST_LINE;
	this->keep_alive = false;
	this->buffer.clear();
}

bool Request::not_allowed_char_in_URL()
{
	for(std::string::iterator it = this->path.begin(); it != this->path.end(); ++it)
	{
		if (!((*it >= '#' && *it <= ';') || (*it >= '?' && *it <= '[') || (*it >= 'a' && *it <= 'z') ||
       *it == '!' || *it == '=' || *it == ']' || *it == '_' || *it == '~'))
			return true;			
	}
	return false;
}

bool Request::not_allowed_char_in_field(std::string value)
{
	//TODO check this for wrong char ....
	for(std::string::iterator it = value.begin(); it != value.end(); ++it)
	{
    if (!(*it == '!' || (*it >= '#' && *it <= '\'') || *it == '*'|| *it == '+' || *it == '-'  || *it == '.' ||
       (*it >= '0' && *it <= '9') || (*it >= 'A' && *it <= 'Z') || (*it >= '^' && *it <= '`') ||
       (*it >= 'a' && *it <= 'z') || *it == '|'))
	   {
			return false;
	   }
			
	}
	return false;	
}