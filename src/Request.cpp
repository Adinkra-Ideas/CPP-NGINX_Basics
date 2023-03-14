#include "Request.hpp"

// Constructors
Request::Request() :
	parse_status(FIRST_LINE)
{
	gettimeofday(&this->start_timer, NULL);
	error_code = NONE;
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
	error_code(copy.error_code)
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
		}
		return *this;
}

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
		this->path = str;
	//TODO check more path and query
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
		value = this->buffer.substr(delimiter + 2, end - delimiter - 2);
		this->headers[key] = value;				// we need to print out what'S stored in headers map object
		start = end + 2;
		end = this->buffer.find_first_of(EOL, start);
	}
	// std::cout << "map header holds: \n";
	// for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it )
	// 	std::cout << "Key= " << (it->first) << " value= " << (it->second) << std::endl;

	buffer.erase(0, end + 2);
	this->parse_status = PREBODY;
	return (this->error_code);
}

int Request::prepare_for_body()
{
	//TODO check if header is ok
	//TODO prepare chunk receiving
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

// how to get request body?
int Request::parse_body()
{
	//TODO check and get request body
	this->parse_status = COMPLETED;
	return (0);
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
	this->buffer.clear();
}
