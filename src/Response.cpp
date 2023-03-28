#include "../includes/Response.hpp"

namespace http {

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
		ErrorCode			status = NONE;

		if ( _request.readStatusCode() == NONE )
		{
			//TODO very simple way of checking if cgi just checking if cgi-bin is in path
			if (this->_request.readPath().find("/cgi-bin") != std::string::npos)
			{
				Cgi cgi_request(this->_request);
				status = cgi_request.getErrorCode();
				this->_web_page = cgi_request.getBody();
			}
			else if ( _request.readMethod() == GET )
					status = doGetPost(_loc_file_path, "GET");
				else if ( _request.readMethod() == POST )
					status = doGetPost(_loc_file_path, "POST");
				// else if ( _request.readMethod() == DELETE )
					// status = respondDeleteRequest();
				else
					status = METHODNOTALLOWED;
		} else
			status = _request.readStatusCode();	// we retrieve status code set from httprequest

		if ( status != OK && status != MOVEDPERMANENTLY && status != FOUND )
			buildErrorCodePage(_web_page, status);

		// ERROR 413 should have been set from httpRequest

		// Add httpResponse status line to stream z.B [HTTP/1.1 200 OK]
		// Add content-type to stream z.B [Content-Type: text/html]
		tmp << _request.readProtocol() << " "
			<< status << " "
			<< ft::translateErrorCode(status) << "\n"
			<< "Content-Type: " << getContentType(_loc_file_path, status) << " \n"
			<< "Content-Length: " << _web_page.size() << " \n"
			<< "Location: " << _location << "\n\n"
			<< _web_page.c_str();

		_response_content = tmp.str();

		_request.clear();
		_loc_file_path.clear();
		_web_page.clear();
		_location.clear();
	}

	// ******************************************************************************
	// handles the GET/POST request. It uses the Location context (of the Server	*
	// that received request from incoming client) to calculate the appropriate		*
	// httpResponse status code. Then if the status code is 200(OK), it retrieves	*
	// the webpage file from local filepath (depending on the GET path of 			*
	// the client's httpRequest data). 												*
	// Return value is the status code of the processed httpRequest					*
	// ******************************************************************************
	ErrorCode	Response::doGetPost( std::string& loc_file_path, const char *method ) {
		std::string		dir_sign;		// z.B. '/'
		std::string		web_url_path;	// stores the website child page requested by client
		std::size_t		pos;

		dir_sign = _request.readPath();
		// parseUrl(dir_sign);	// for removing the %%%% and other unformated chars

		// erase spaces at the beginning (if any)
		pos = dir_sign.find_first_not_of(' ');
		if ( pos )
			dir_sign.erase(0, pos);
		
		// put '/' at the beginning if absent
		if ( dir_sign.size() > 0 && dir_sign.at(0) != '/' )
			dir_sign.insert(0, 1, '/');
		
		// backup name=>value if any
		if ( _request.readQuery().size() > 0 ) {
			std::ofstream 	_fout;
			_fout.open("queryData/allGetQeries", std::ios::out | std::ios::app );
			if (! _fout.good() )
				print_status(ft_RED, "Please Manually Create Dir queryData/");
			_fout << _request.readQuery() << std::endl;
			_fout.close();
		} else if ( _request.getRequestBody().size() > 0 )
			collatePostQuery(_request.getRequestBody());



		// In the client's http request, split the directory from path. 
		// Store directory to dir_sign and filename to web_url_path
		if ( dir_sign.size() > 0) {
			pos = dir_sign.find_first_of('/');
			web_url_path = dir_sign.substr(pos);
			dir_sign.erase(pos + 1);
			pos = web_url_path.size();
			if ( pos > 2 && web_url_path.at(pos - 1) == '/' )
				web_url_path.erase(pos - 1, 1);	// we want to search Location context using z.B '/web_url_path' not '/web_url_path/'
		} else {
			return NOTFOUND;
		}

		// we will search through the Location contexts to see if web_url_path has
		// its own specific Location context set. If none found, we will repeat
		// search using just dir_sign (AKA "/")
		std::string								*ptr = &web_url_path;
		bool									flag = false;
		std::vector<http::Location>::iterator	it = _server->refLocations().begin();
		while ( it != _server->refLocations().end() ) {
			if ( ! it->readPath().compare(*ptr) && (std::find(it->refMethods().begin(),
						it->refMethods().end(), method) != it->refMethods().end()) )		// Location's method must support GET
				break ;
			++it;
			if ( it == _server->refLocations().end() && !flag ) {
				flag = true;
				it = _server->refLocations().begin();
				ptr = &dir_sign;
			}
		}
		if ( it == _server->refLocations().end() )
			return METHODNOTALLOWED;
		else {
			loc_file_path.append(it->readRoot());
			loc_file_path.append(web_url_path);
			_root_directory = it->readRoot();
		}

		// check if any redirection is present
		ErrorCode	status = NONE;
		if ( (status = check_for_redirections(loc_file_path, web_url_path, it)) != NONE )
			return status; 

		// Open an input file stream, write to stream from loc_file_path,
		// if write failed, return error404, else read from stream to 
		// _web_page and return status 200
		{
			std::ifstream		fin;
			fin.open(loc_file_path.c_str());
			if ( ! fin.good() )
				return NOTFOUND;
			std::stringstream buff_tmp;
			buff_tmp << fin.rdbuf();
			fin.close();
			// It is at this point that PHP or other backend script are exec'd on the file
			// exec_php(buff_tmp.str(), loc_file_path);
			_web_page.append(buff_tmp.str().c_str());
			// WE WILL ALSO RECHECK MAXBODY HERE
		}
		return OK;
	}

	// collate query parameters of POST requests and do necessary dos
	void	Response::collatePostQuery( const std::string& post_query ) {
		std::size_t		pos = 0;
		std::string		file_name;
		std::string		tmp;

		std::ofstream 	_fout;
		_fout.open("queryData/allPostQeries", std::ios::out | std::ios::app );
		if (! _fout.good() ) {
			print_status(ft_RED, "Please Manually Create Dir queryData/");
			return ;
		}

		// WebKitFormXXX might be peculiar to only POST queries
		// sent from a Chrome Browser
		while ( (pos = post_query.find("form-data;", pos)) != std::string::npos ) {
			if ( (pos = post_query.find("name=", pos)) != std::string::npos ) {
				pos += std::strlen("name=\"");
				_fout << post_query.substr(pos, post_query.find('"', pos) - pos)
						<< "=";
				pos = post_query.find('"', pos) + 1;

				// check if a file was uploaded
				if ( !post_query.compare(pos, 11 ,"; filename=") ) {	// 11 == std::strlen("; filename=")
					pos = post_query.find('"', pos) + 1;
					tmp = post_query.substr(pos, post_query.find('"', pos) - pos);
					if ( (pos = post_query.find_first_not_of("\r\n", pos + tmp.size() + 1)) != std::string::npos )
						_fout << post_query.substr(pos, post_query.find("\r\n", pos) - pos) << "\r\n";

					tmp.insert(0, "queryData/");
					std::ofstream 	_uploaded_file;
					_uploaded_file.open(tmp.c_str(), std::ios::out | std::ios::trunc );
					if ( _uploaded_file.good() ) {
						if ( (pos = post_query.find("\r\n\r\n", pos)) != std::string::npos ) {
							pos += std::strlen("\r\n\r\n");
							_uploaded_file << post_query.substr(pos, post_query.find("\r\n------WebKitForm", pos) - pos);
						}
						_uploaded_file.close();
					}
				}
				//else check if the key has a value
				else if ( !post_query.compare(pos, 4,"\r\n\r\n") ) {	//( (pos = post_query.find("\r\n\r\n", pos)) != std::string::npos ) {
					pos += 4;
					// pos = post_query.find_first_not_of("\r\n", pos);
					_fout << post_query.substr(pos, post_query.find("------WebKitForm", pos) - pos);
				}
			}
		}
		_fout.close();
	}

	// **************************************************************
	// used for building the "Content-Type: " part of httpresponse	*
	// Body. Return Value is the appropriate browser compatible 	*
	// content-type.												*
	// **************************************************************
	std::string	Response::getContentType( const std::string& loc_file_path, const ErrorCode& status ) {
		std::string		tmp;
		std::size_t		pos;

		// Checking if we are returning an error page to 
		if ( status != OK || status != MOVEDPERMANENTLY || status != FOUND )  {
			tmp.insert( 0, "text/html" );
			return tmp;
		}

		// Getting file extension stored to tmp
		if ( (pos = loc_file_path.find_last_of('.')) != std::string::npos )
			tmp = loc_file_path.substr(pos);
		
		// retrieving the necessary browser-compatible content-type description
		tmp = mime::getMimeType(tmp);
		return tmp;
	}

	// **********************************************************************
	// use supported error code to build their corresponding webpages		*
	// just name the error code file the "error_number.html" and it will	*
	// automatically be picked by this function								*
	// **********************************************************************
	void	Response::buildErrorCodePage(std::string& web_page, ErrorCode& status) {
		std::ostringstream	buff_tmp;
		std::ifstream		fin;
		std::string			error_pages;

		// if we have a LISTDIRECTORYCONTENT request, do 
		if ( status == LISTDIRECTORYCONTENTS ) {
			ft::listDirectoryContent(web_page, _loc_file_path, _root_directory);
			status = OK;
			return;
		}

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
		} else
			web_page.insert(0, "<!DOCTYPE html><head><title>Unknown Error</title></head><body>Unknown Error</body></html>");
	}	

	// ******************************************************************************
	// 'loc_file_path' holds the local address that we're checking for redirects.	*
	// 'web_url_path' is the web URL that is used for reaching loc_file_path.		*
	// 'it' is the iterator that is currently stuck at the Location context which	*
	// is used to determine the index, root and other directives from building this	*
	// response.																	*
	// If loc_file_path is a directory and it has a readable index file, we set 	*
	// _location to the 'web_url_path + index file' and set the status code to 301.	*
	// But if loc_file_path is a directory which has no/unreadable index file, we 	*
	// return error 403																*
	// ******************************************************************************
	ErrorCode	Response::check_for_redirections(std::string& loc_file_path,
				std::string& web_url_path, std::vector<http::Location>::iterator& it) {
		std::size_t		pos;

		// Check if 301 redirection is necessary for Client's requested path
		// If yes, set status to MOVEDPERMANENTLY and store new Web URL to _location
		if ( ft::isDirectory(loc_file_path) ) {
			pos = web_url_path.size();
			if ( pos > 2 && web_url_path.at(pos - 1) != '/' )	// pos > 2 means web_url_path must hold at least "/xx" chars count
				web_url_path.push_back('/');

			_location.append(web_url_path);

			// if it's a dir and no index file found in Location context, or index file not readable
			{
				std::ifstream		fin;
				std::string			tmp_local_path = loc_file_path;

				if ( ! tmp_local_path.empty() && *tmp_local_path.end() - 1 != '/' )
					tmp_local_path.push_back('/');
				tmp_local_path.append( ((it->readIndex().size() > 0) ? it->readIndex() : ";)") );
				fin.open(tmp_local_path.c_str());
				if ( ! fin.good() ) {
					_location.clear();

					// if autoindex is "on", directories that do not have an index.html file will 
					// will list their contents intead of showing "403 forbidden" error
					if ( ! it->readAutoind().compare("on") ) {
						return LISTDIRECTORYCONTENTS;
					}
					
					return FORBIDDEN;
				}
			}

			_location.append(it->readIndex());
			return MOVEDPERMANENTLY;	
		}

		// Check if 302 (AKA http redirect) redirection is present in route/location
		else if ( it->readRewrite().size() ) {
			_location.append(it->readRewrite().c_str());
			return FOUND;
		}
		return NONE;
	}

}	// namespace http
