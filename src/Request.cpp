#include "Request.hpp"

// Constructors
Request::Request() :
	parse_status(FIRST_LINE),
	protocol("HTTP/1.1"),
	keep_alive(true),
	request_started(false)
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
	cgi_method(copy.cgi_method),
	max_body_size(copy.max_body_size),
	request_started(copy.request_started)
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
			this->max_body_size = assign.max_body_size;
			this->request_started = assign.request_started;
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

// Returns the string after the first occurence of '?' (ifExist) 
// from path to doc requested by client
const std::string&	Request::readQuery( void ) { return this->query; }

// Returns the content of the _method (AKA GET, POST, or DELETE)
const Method&	Request::readMethod( void ) { return method; }

// Returns the status code of the httprequest
const ErrorCode&	Request::readStatusCode( void ) { return error_code; }
void	Request::setStatusCode( const ErrorCode error ) { this->error_code = error; }
void Request::setCgi_exe(std::string str) {this->cgi_exe = str;}
std::string	Request::getCgi_exe() {return this->cgi_exe;}

void Request::setCgi_method(std::string str) {this->cgi_method = str;}
std::string	Request::getCgi_method() {return this->cgi_method;}

// returns the headers map object
const std::map<std::string, std::string>&	Request::readHeaders( void ) { return headers; }

std::string& Request::getRequestBody()
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

void Request::parse(std::string &buffer)
{
	// std::cout << "\n\n\nTheHeader" << buffer << std::endl;
	request_started = true;
	this->buffer += buffer;
	buffer.clear();
	// parse_status is set to FIRST_LINE as set by Request default constructor from client.hpp
	// then if first line of client request was parsed successfully, parse_status is set to HEADERS
	if (parse_status == FIRST_LINE)
		first_line();
	if (parse_status == HEADERS)
		parse_headers();
	if (parse_status == PREBODY)
		prepare_for_body();
	if (parse_status == BODY)
		parse_body();
	if (parse_status == CHUNK)			// what does chunk do/mean?
		parse_chunks();
}

void Request::first_line()
{
	if (this->buffer.find(EOL) != std::string::npos)   // EOL == '\r\n'
	{
		size_t start = 0;
		size_t end = this->buffer.find_first_of(" ", start);
		parseMethod(this->buffer.substr(start, end));
		if (this->error_code)
			return ;
		start = end + 1;
		end = this->buffer.find_first_of(" ", start);
		parsePath(this->buffer.substr(start, end - start));
		if (this->error_code)
			return ;
		start = end + 1;
		end = this->buffer.find_first_of(EOL, start);
		parseProtocol(this->buffer.substr(start, end - start));
		if (this->error_code)
			return ;
		this->buffer.erase(0, end + 2);
		this->parse_status = HEADERS;
	}
}
void Request::parseProtocol(std::string str)
{
	if (str.compare("HTTP/1.1"))
		this->error_code = HTTPVERSIONNOTSUPPORTED;
}

void Request::parseMethod(std::string str)
{
	if (!str.compare("GET"))
		this->method = GET;
	else if (!str.compare("POST"))
		this->method = POST;
	else if (!str.compare("DELETE"))
		this->method = DELETE;
	else if (!str.compare("HEAD"))
		this->method = HEAD;
	else if (!str.compare("PUT"))
		this->method = PUT;
	else 
		this->error_code = BADREQUEST;
}

void Request::parsePath(std::string str)
{
	if (str.empty() || str.at(0) != '/')
		this->error_code = BADREQUEST;
	else if (str.length() > MAXURISIZE)
		this->error_code = URITOOLONG;
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
			http::print_status(ft_RED, "Not allowed char in the url");
			this->error_code = BADREQUEST;
		}
	}
}

void Request::parse_headers()
{
		size_t end = this->buffer.find(EOL);
	size_t delimiter;
	std::string key;
	std::string value;
	// copy all the lines below line one from request Header
	// to a map of Name=>Value
	while (end != std::string::npos)
	{
		if(this->buffer.find(EOL) == 0)
		{
				buffer.erase(0, end + 2);
				this->parse_status = PREBODY;
				break ;
		}		
		delimiter = this->buffer.find(':');
		if (delimiter == std::string::npos)
		{
			this->parse_status = COMPLETED;
			this->error_code = BADREQUEST;
			return ;
		}
		key = http::to_lower_case(this->buffer.substr(0, delimiter));
		if(key.empty() || not_allowed_char_in_key(key))
		{
			this->parse_status = COMPLETED;
			this->error_code = BADREQUEST;
			return ;
		}
		value = http::trim_whitespace(this->buffer.substr(delimiter + 1, end - delimiter - 1));
		if (value.size() > MAXVALUESIZE || not_allowed_char_in_field(value) )
		{
			this->parse_status = COMPLETED;
			this->error_code = BADREQUEST;
			return ;

		}
		if (this->headers.count(key))
		{
			this->parse_status = COMPLETED;
			this->error_code = BADREQUEST;
			return ;
		}
		this->headers[key] = value;
		this->buffer.erase(0, end + 2);
		end = this->buffer.find(EOL);
	}
}



void Request::prepare_for_body()
{
	if (this->headers.find("host") == this->headers.end() || this->headers["host"].empty())
	{
		this->error_code = BADREQUEST;
		this->parse_status = COMPLETED;
		return ;
	}
	else if (this->headers["host"].find('@') != std::string::npos)
	{
		this->error_code = BADREQUEST;
		this->parse_status = COMPLETED;
		return ;
	}
	else
	{
		size_t pos = this->headers["host"].find(':');
		this->serverName = http::trim_whitespace(this->headers["host"].substr(0, pos));
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
		this->body_length = parse_str_to_int(this->headers["content-length"]);
		if (this->body_length == SIZE_MAX_T)
		{
			this->error_code = BADREQUEST;
			this->parse_status = COMPLETED;
			return ;
		}
		if(this->body_length > this->max_body_size)
		{
			this->error_code = CONTENTTOOLARGE;
			this->parse_status = COMPLETED;
			return ;
		}
		this->parse_status = BODY;
	}
	else
	{
			this->parse_status = COMPLETED;
			this->buffer.clear();	
	}
		
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
			return (SIZE_MAX_T);
		if (result > MAXBODYSIZE)
			return (SIZE_MAX_T);
	}
	return (result);
}

void Request::parse_body()
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
}

void Request::parse_chunks()
{
	size_t end;
	while ((end = this->buffer.find(EOL)) != std::string::npos)
	{
		if(this->chunk_part == CHUNKSIZE)
		{
			this->chunk_length = http::str_to_hex(this->buffer.substr(0, end));
			this->buffer.erase(0, end + 2);
			this->chunk_part = CHUNKDATA;
		}
		if(this->chunk_part == CHUNKDATA)
		{
			if (this->chunk_length == 0)
			{
				trailing_chunk();
				return ;
				this->buffer.clear();
			}
			else if(this->buffer.length() >= this->chunk_length + 2)
			{
				this->body += this->buffer.substr(0, this->chunk_length);
				if (this->body.length() > this->max_body_size)
				{
					this->error_code = CONTENTTOOLARGE;
					this->parse_status = COMPLETED;
					return ;
				}
				this->buffer.erase(0, this->chunk_length + 2);
				this->chunk_part = CHUNKSIZE;
			}
			else if ((this->buffer.length() + this->body.length()) > this->max_body_size)
			{
				this->error_code = CONTENTTOOLARGE;
				this->parse_status = COMPLETED;
				return ;
			}
		}
	}
}

bool Request::parsingFinished()
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
	this->parse_status = FIRST_LINE;
	this->buffer.clear();
	this->serverName.clear();
	this->path.clear();
	this->query.clear();
	this->request_body.clear();
	this->headers.clear();
	this->chunk_length = 0;
	this->body_length = 0;
	this->length = 0;
	this->error_code = NONE;
	this->chunk_part = CHUNKSIZE;
	this->body.clear();
	this->keep_alive = true;
	this->cgi_exe.clear();
	this->cgi_method.clear();
	this->request_started = false;
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
    if (!((*it >= ' ' && *it <= '?') || (*it >= 'A'&& *it <= '~')))
	   {
			return true;
	   }
	}
	return false;	
}

bool Request::not_allowed_char_in_key(std::string value)
{
	for(std::string::iterator it = value.begin(); it != value.end(); ++it)
	{
    if (!(*it == '-'  || *it == '_' ||(*it >= '0' && *it <= '9') || (*it >= 'A' && *it <= 'Z') ||
       (*it >= 'a' && *it <= 'z')))
	   {
			return true;
	   }	
	}
	return false;	
}

void Request::set_max_body_size(size_t n)
{
	this->max_body_size = n;
}

bool Request::has_request()
{
	return this->request_started;
}

//TODO check if chunk trailers are fine
void Request::trailing_chunk()
{
	size_t end = this->buffer.find(EOL);
	size_t delimiter;
	std::string key;
	std::string value;
	while (end != std::string::npos)
	{
		if(this->buffer.find(EOL) == 0)
		{
				buffer.clear();
				this->parse_status = COMPLETED;
				break ;
		}
		delimiter = this->buffer.find(':');
		if (delimiter == std::string::npos)
		{
			this->parse_status = COMPLETED;
			this->error_code = BADREQUEST;
			return ;
		}
		key = http::to_lower_case(this->buffer.substr(0, delimiter));
		if(key.empty() || not_allowed_char_in_key(key))
		{
			this->parse_status = COMPLETED;
			this->error_code = BADREQUEST;
			return ;
		}	
		this->buffer.erase(0, end + 2);
		end = this->buffer.find(EOL);
	}
}
