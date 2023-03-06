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

	if (parse_status == FIRST_LINE)
		error_lvl = first_line();
	if (parse_status == HEADERS)
		error_lvl = parse_headers();
	if (parse_status == PREBODY)
		error_lvl = prepare_for_body();
	if (parse_status == BODY)
		error_lvl = parse_body();
	if (parse_status == CHUNK)
		error_lvl = parse_chunks();
	return (error_lvl);
}

//TODO also check total length of the request
int Request::first_line()
{
	if (this->buffer.find(EOL) != std::string::npos)
	{
		//TODO parse method
		this->method = GET;
		//TODO check and parse path + query
		this->path = "/";
		//TODO check protocol
		this->protocol = "HTTP/1.1";
		this->parse_status = HEADERS;
	}
	
	return (0);
}

int Request::parse_headers()
{
	//TODO parse header for atleast Host
	//TODO extra stuff to parse for: Transfer-Encoding, Content-Length, Connection
	this->headers["Host"] = "127.0.0.1:7655";
	this->parse_status = PREBODY;
	return (0);
}

int Request::prepare_for_body()
{
	//TODO check if header is ok
	//TODO prepare chunk receiving
	this->parse_status = BODY;
	return (0);
}

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

bool Request::parsingFinished()
{
	return (this->parse_status == COMPLETED);
}

std::string Request::getServerName()
{
	return (this->serverName);
}
