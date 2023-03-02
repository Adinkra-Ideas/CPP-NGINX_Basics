#include "../includes/Response.hpp"

// Constructors
Response::Response()
{
}

Response::Response(const Response &copy)
{
	(void) copy;
}


// Destructor
Response::~Response()
{
}


// Operators
Response & Response::operator=(const Response &assign)
{
	(void) assign;
	return *this;
}
