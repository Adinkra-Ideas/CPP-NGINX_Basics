#include "Request.hpp"

// Constructors
Request::Request()
{
}

Request::Request(const Request &copy)
{
	(void) copy;
}


// Destructor
Request::~Request()
{
}


// Operators
Request & Request::operator=(const Request &assign)
{
	(void) assign;
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

int Request::first_line()
{
	std::cout << "First line" << std::endl;
	this->parse_status = HEADERS;
	return (0);
}

int Request::parse_headers()
{
	std::cout << "Headers" << std::endl;
	this->parse_status = PREBODY;
	return (0);
}

int Request::prepare_for_body()
{
	std::cout << "get ready for body" << std::endl;
	this->parse_status = BODY;
	return (0);
}

int Request::parse_body()
{
	std::cout << "Body parsing" << std::endl;
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
