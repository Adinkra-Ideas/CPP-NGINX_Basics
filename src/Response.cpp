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
	_request = request;
}

void Response::setServer(http::Server &server)
{
	_server = &server;
}

std::string& Response::refResponseCont( void ) { return _response_content; }

//TODO building the response
void Response::buildResponse( void )
{
	std::ostringstream	tmp;
	ErrorCode			status_code = NONE;

	// Using the right Method to process _requested filepath
	if ( _request.readMethod() == GET )
		status_code = processGetRequest(_requested_file);
	// else if ( _request.readMethod() == POST )
	// 	status_code = processPostRequest();
	// else if ( _request.readMethod() == DELETE )
	// 	status_code = processDeleteRequest();

	buildErrorCodePage(_web_page, status_code);

	// Add httpResponse status line to stream z.B [HTTP/1.1 200 OK]
	// Add content-type to stream z.B [Content-Type: text/html]
	tmp << _request.readProtocol() << " "
		<< status_code << " "
		<< translateErrorCode(status_code) << "\n"
		<< "Content-Type: " << getContentType(_requested_file, status_code) << " \n"
		<< "Content-Length: " << _web_page.size() << "\n\n";

	tmp << _web_page.c_str();
	_response_content = tmp.str();

	_request.clear();
	_requested_file.clear();
	_web_page.clear();
}

// **********************************************************************
// use supported error code to build their corresponding webpages		*
// just name the error code file the "error_number.html" and it will	*
// automatically be picked by this function								*
// **********************************************************************
const std::string&	Response::buildErrorCodePage(std::string& web_page, const ErrorCode& status) {
	std::stringstream	buff_tmp;
	std::ifstream		fin;

	if ( status == OK )
		return web_page;

	buff_tmp << "public_html/" << status << ".html";
	fin.open( buff_tmp.str().c_str() );
	if ( ! fin.good() )
		web_page.insert(0, "<!DOCTYPE html><html lang=\"en\"><head><title>404 NOT FOUND</title></head><body><h1>404 NOT FOUND</h1></body></html>");
	else {
		buff_tmp.str("");
		buff_tmp << fin.rdbuf();
		fin.close();
		web_page.insert(0, buff_tmp.str().c_str() );
	}
	return web_page;
}

// ******************************************************************************
// handles the GET method request. It uses the Location context (of the Server	*
// that received request from incoming client) to calculate the appropriate		*
// httpResponse status code. Then if the status code is 200(OK), it retrieves	*
// the webpage file from local filepath (depending on the GET path of 			*
// the client's httpRequest data). 												*
// Return value is the status code of the processed httpRequest					*
// ******************************************************************************
ErrorCode	Response::processGetRequest( std::string& requested_file) {
	std::string		dir_tmp;
	std::string		file_path;
	std::size_t		pos;

	dir_tmp = _request.readPath();

	// If http request is requesting a path to a file, split
	// the directory from path. Store directory to dir_tmp and
	// filename to file_path
	if ( dir_tmp.size() > 0) {
		if ( (pos = dir_tmp.find_first_of('/')) != std::string::npos ) {
			dir_tmp.erase(0, pos);
			pos = dir_tmp.find_first_of('/');
			file_path = dir_tmp.substr(pos + 1);
			dir_tmp.erase(pos + 1);
		} else {
			return NOTFOUND;	// This return value is a placeholder. Might be subject to change later on
		}
	}

	// checking if dir_tmp is an existing Location context in _server
	std::vector<http::Location>::iterator it = _server->refLocations().begin();
	for ( ; it != _server->refLocations().end(); ++it )
		if ( ! it->readPath().compare(dir_tmp) )
			break ;

	// building the _requested_file path for client's request. But first
	// we check if dir_tmp wasn't found among Location context in _server
	if ( it == _server->refLocations().end() )
		return NOTFOUND;
	else {
		requested_file.insert(0, it->readRoot());
		pos = requested_file.size();
		requested_file.insert(pos, dir_tmp);
		pos = requested_file.size();
		requested_file.insert(pos, ( (file_path.size() > 0) ? file_path : it->readIndex() ) );	// get me filepath that client requested or if none, get me the index file in Location
	}

	// Open an input file stream, write to stream from requested_file,
	// if write failed, return error404, else read from stream to 
	// _web_page and return status 200
	{
		std::ifstream		fin;
		fin.open(requested_file.c_str());
		if ( ! fin.good() )
			return NOTFOUND;				//dont know if returning from within a nested quote might cause UB
		std::stringstream buff_tmp;
		buff_tmp << fin.rdbuf();
		fin.close();
		_web_page.insert(0, buff_tmp.str().c_str());
	}
	return OK;
}

// **************************************************************
// used for building the "Content-Type: " part of httpresponse	*
// Body. Return Value is the appropriate browser compatible 	*
// content-type.												*
// **************************************************************
std::string	Response::getContentType( const std::string& requested_file, const ErrorCode& status ) {
	std::string		tmp;
	std::size_t		pos;

	if ( status == NOTFOUND )  {// or other unknown conditions
		tmp.insert( 0, "text/html" );
		return tmp;
	}

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
