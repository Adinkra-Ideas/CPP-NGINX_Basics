#include "../includes/Response.hpp"
#include <sstream>
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
void Response::buildResponse()
{
	// we need a minimized webpage with around 5 pages including one that has an upload form tag
	// then we can map<std::string, std::string> filepath=>Webpage doc
	
	std::string	webpage("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"https://fonts.googleapis.com/css2?family=Kaushan+Script&family=Montserrat:wght@400;700&display=swap\" rel=\"stylesheet\"><link href=\"https://drive.google.com/uc?export=view&id=1Xugr6sMP2KHXBkaLNlZFxRnBLNDQkB5R\" rel=\"stylesheet\"><title>Fusion</title></head><body><header class=\"header\"><div class=\"container\"><div class=\"header__inner\"><a class=\"nav__link\" href=\"#\">Tour</a><nav class=\"nav\"><a class=\"nav__link\" href=\"#\">Home</a><a class=\"nav__link\" href=\"#\">Services</a><a class=\"nav__link\" href=\"#\">About</a><a class=\"nav__link\" href=\"#\">Contact</a><a class=\"nav__link\" href=\"#\">Cookies Test</a><a class=\"nav__link\" href=\"#\">Account</a></nav></div></div></header><div class=\"intro\"><div class=\"container\"><div class=\"intro__inner\"><h2 class=\"inner__uptitle\">Fusion travel</h2><h1 class=\"intro__title\">Let's Enjoy Your Trip In UAE</h1></div></div><footer><p><span class=\"highlight\">&#169; 2022 by AMANIX</p></footer></div></body></html>");
	std::ostringstream tmp;
	std::string httpResponse;

	tmp << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "
		<< webpage.size() << "\n\n" << webpage;
	this->response_content = tmp.str();
}