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

void Response::setRequest(Request &request)
{
	this->request = request;
}

void Response::setServer(http::Server &server)
{
	this->server = server;
}

//TODO building the response
void Response::buildResponse( void )
{
	std::ostringstream	tmp;
	ErrorCode			status_code = NONE;

	// Using the right Method to process requested filepath
	if ( request.readMethod() == GET )
		status_code = processGetRequest(_requested_file);
	// else if ( request.readMethod() == POST )
	// 	status_code = processPostRequest();
	// else if ( request.readMethod() == DELETE )
	// 	status_code = processDeleteRequest();

	// Add httpResponse status line to stream z.B [HTTP/1.1 200 OK]
	// Add content-type to stream z.B [Content-Type: text/html]
	tmp << request.readProtocol() << " "
		<< status_code << " "
		<< translateErrorCode(status_code) << "\n"
		<< "Content-Type: " << getContentType(_requested_file) << " \n"  ;
	
	// The below path will be changed to dynamic after we've built requested filepath
	// in the _requested_file string object
	std::string	webpage("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"https://fonts.googleapis.com/css2?family=Kaushan+Script&family=Montserrat:wght@400;700&display=swap\" rel=\"stylesheet\"><link href=\"https://drive.google.com/uc?export=view&id=1Xugr6sMP2KHXBkaLNlZFxRnBLNDQkB5R\" rel=\"stylesheet\"><title>Fusion</title></head><body><header class=\"header\"><div class=\"container\"><div class=\"header__inner\"><a class=\"nav__link\" href=\"#\">Tour</a><nav class=\"nav\"><a class=\"nav__link\" href=\"#\">Home</a><a class=\"nav__link\" href=\"#\">Services</a><a class=\"nav__link\" href=\"#\">About</a><a class=\"nav__link\" href=\"#\">Contact</a><a class=\"nav__link\" href=\"#\">Cookies Test</a><a class=\"nav__link\" href=\"#\">Account</a></nav></div></div></header><div class=\"intro\"><div class=\"container\"><div class=\"intro__inner\"><h2 class=\"inner__uptitle\">Fusion travel</h2><h1 class=\"intro__title\">Let's Enjoy Your Trip In UAE</h1></div></div><footer><p><span class=\"highlight\">&#169; 2022 by AMANIX</p></footer></div></body></html>");

	// tmp << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "
	tmp << webpage.size() << "\n\n" << webpage;
	this->response_content = tmp.str();
}

ErrorCode	Response::processGetRequest( std::string& requested_file ) {

	// This is just a dummy sentence to add a temporary filepath in _requested_file for the meantime.
	// Ideally we will retrieve the file pathe requested from client, compare
	// against server config to determine how the filepath will be constructed
	requested_file.insert(0, "home/dummyfile.html" );

	// The return value below is also a dummy. 
	// will continue from this function tomorrow.
	// The question now is what logic will be used to get the filepath
	// because at the moment, the requested path is captured already.
	// But how do we now uses the server config to build this filepath?
	return OK;

}

// **************************************************************
// used for building the "Content-Type: " part of httpresponse	*
// Body. Return Value is the appropriate browser compatible 	*
// content-type.												*
// **************************************************************
std::string	Response::getContentType( const std::string& requested_file ) {
	std::string		tmp;
	std::size_t		pos;

	// Getting file extension stored to tmp
	if ( (pos = requested_file.find_last_of('.')) != std::string::npos )
		tmp = requested_file.substr(pos + 1);
	
	// Adding the necessary browser-compatible content-type description
	if ( ! tmp.compare("html") )
		tmp.insert( 0, "text/" );
	// else if				// for adding more supported content-types


	return tmp;
}

// ******************************************************************
// Converts the error code stored in status_code into the readable	*
// error message. status_code is a var of data type ErrorCode enum	*
// ******************************************************************
std::string	Response::translateErrorCode( const ErrorCode& status_code ) {
	switch (status_code) {
		case 200:
			return "OK";
		case 404:
			return "NOT FOUND";
		// The rest cases goes here
		default:
			return "UNKNOWN RESPONSE";	// Optional
	}
}
