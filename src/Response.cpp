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

// TODO building the response
void Response::buildResponse( void )
{
	std::ostringstream	tmp;
	ErrorCode			status_code = NONE;

	// Using the right Method to process _requested filepath
	if ( _request.readMethod() == GET )
		status_code = respondGetRequest(_requested_file);
	// else if ( _request.readMethod() == POST )
	// 	status_code = respondPostRequest();
	// else if ( _request.readMethod() == DELETE )
	// 	status_code = respondDeleteRequest();
	else
		status_code = METHODNOTALLOWED;

	buildErrorCodePage(_web_page, status_code);

	// ERROR 413 goes here if 

	// Add httpResponse status line to stream z.B [HTTP/1.1 200 OK]
	// Add content-type to stream z.B [Content-Type: text/html]
	tmp << _request.readProtocol() << " "
		<< status_code << " "
		<< translateErrorCode(status_code) << "\n"
		<< "Content-Type: " << getContentType(_requested_file, status_code) << " \n"
		<< "Content-Length: " << _web_page.size() << " \n"
		<< "Location: " << _location << "\n\n";

	tmp << _web_page.c_str();
	_response_content = tmp.str();

	_request.clear();
	_requested_file.clear();
	_web_page.clear();
	_location.clear();
}

// **********************************************************************
// use supported error code to build their corresponding webpages		*
// just name the error code file the "error_number.html" and it will	*
// automatically be picked by this function								*
// **********************************************************************
const std::string&	Response::buildErrorCodePage(std::string& web_page, const ErrorCode& status) {
	std::stringstream	buff_tmp;
	std::ifstream		fin;
	std::string			error_pages;

	// if request is okay or there was a redirection, no need to build an error page
	if ( status == OK || status == FOUND || status == MOVEDPERMANENTLY )
		return web_page;

	// assign the error_page value from server config to error_pages,
	// if error_pages.size() <= 0, use our default error page directory
	// else, use the error_page value from server config
	error_pages = _server->readErrorPage();
	if ( error_pages.size() <= 0 )
		buff_tmp << "public_html/error_pages/" << status << ".html";
	else
		buff_tmp << error_pages.c_str() << "/" << status << ".html";
	
	fin.open( buff_tmp.str().c_str() );
	if ( fin.good() ) {
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
ErrorCode	Response::respondGetRequest( std::string& requested_file) {
	std::string		dir_tmp;		// z.B. /public/index.html (path retrieved from client'S request header)
	std::string		file_path;		// stores the website child page requested by client without the '/' at the beginning
	std::size_t		pos;

	dir_tmp = _request.readPath();

	// erase spaces at the beginning (if any)
	pos = dir_tmp.find_first_not_of(' ');
	if ( pos )
		dir_tmp.erase(0, pos);

	// Check if 302 redirection is necessary for Client's requested path
	// If yes, store new Web URL to _location and return FOUND
	if ( dir_tmp.size() > 0 && dir_tmp.at(0) != '/' ) {
		dir_tmp.insert(0, 0, '/');
		_location.insert(0, "http://");
		_location.append(_server->readIp());
		_location.push_back(':');
		{
			std::ostringstream	tmp;
			tmp << _server->readPort();
			_location.append(tmp.str());
		}
		_location.append(dir_tmp);
		return FOUND;
	} 

	// std::cout << "new location " << _location << std::endl;

	// In the client's http request, split
	// the directory from path. Store directory to dir_tmp and
	// filename to file_path
	if ( dir_tmp.size() > 0) {
		pos = dir_tmp.find_first_of('/');
		file_path = dir_tmp.substr(pos + 1);
		dir_tmp.erase(pos + 1);
	} else {
		return NOTFOUND;
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
		// requested_file.insert(pos, ( (file_path.size() > 0) ? file_path : it->readIndex() ) );	// get me filepath that client requested or if none, get me the index file in Location
		requested_file.insert(pos, file_path);
	}

	// Check if 301 redirection is necessary for Client's requested path
	// If yes, set status to FOUND and store new Web URL to _location
	if ( isDirectory(requested_file) ) {
		std::cout << "it is a directory" << std::endl;

		pos = file_path.size();
		if ( pos > 0 && file_path.at(pos - 1) != '/' )
			file_path.push_back('/');
	
		_location.insert(0, "http://");
		_location.append(_server->readIp());
		_location.push_back(':');
		{
			std::ostringstream	tmp;
			tmp << _server->readPort();
			_location.append(tmp.str());
		}
		_location.append(dir_tmp);
		_location.append(file_path);
		_location.append(it->readIndex());
		return MOVEDPERMANENTLY;	
	}

	// Open an input file stream, write to stream from requested_file,
	// if write failed, return error404, else read from stream to 
	// _web_page and return status 200
	{
		std::ifstream		fin;
		fin.open(requested_file.c_str());
		if ( ! fin.good() )
			return NOTFOUND;
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

	// Checking if we are returning an error page to 
	if ( status != OK || status != FOUND || status != MOVEDPERMANENTLY)  {
		tmp.insert( 0, "text/html" );
		return tmp;
	}

	// Getting file extension stored to tmp
	if ( (pos = requested_file.find_last_of('.')) != std::string::npos )
		tmp = requested_file.substr(pos);
	
	// retrieving the necessary browser-compatible content-type description
	tmp = http::mime::getMimeType(tmp);
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
		case 301:
			return "MOVEDPERMANENTLY";
		case 302:
			return "FOUND";
		case 404:
			return "NOT FOUND";
		case 405:
			return "METHOD NOT ALLOWED";
		// The rest cases goes here
		default:
			return "UNKNOWN RESPONSE";	// Optional
	}
}

// Only returns true when path is a directory
bool Response::isDirectory(const std::string &path)
{
    struct stat file_stat;

    if (stat(path.c_str(), &file_stat) != 0)
        return (false);

    return (S_ISDIR(file_stat.st_mode));
}
