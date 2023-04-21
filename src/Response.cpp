#include "../includes/Response.hpp"

namespace http {

	// Constructors
	Response::Response( void ) : _request(NULL), _server(NULL),
			_loc_file_path(), _web_page(), _response_content(),
			_location(), _root_directory(), byte_send(0)
	{
	}

	Response::Response( const Response &other ) : _request(other._request), 
			_server(other._server), _loc_file_path(other._loc_file_path),
			_web_page(other._web_page), _response_content(other._response_content),
			_location(other._location), _root_directory(other._root_directory),
			byte_send(other.byte_send)
	{
	}

	// Destructor
	Response::~Response( void )
	{
	}

	// Operators
	Response & Response::operator=( const Response &other )
	{
		if ( this != &other ) {
			_request = other._request; 
			_server = other._server;
			_loc_file_path = other._loc_file_path;
			_web_page = other._web_page;
			_response_content = other._response_content;
			_location = other._location;
			_root_directory = other._root_directory;
			byte_send = other.byte_send;
		}
		return *this;
	}

	// ****************	CONSTRUCTORS && OPERATORS ENDS	**********************
	//////////////////////////////////////////////////////////////////////////
	// ******************* READING AND WRITING BEGINS	**********************
	void Response::setRequest(Request &request)
	{
		_request = &request;
	}

	void Response::setServer(http::Server &server)
	{
		_server = &server;
	}

	// returns a reference to the _response_content member
	std::string& Response::refResponseCont( void ) { return _response_content; }

	// ****************	READING AND WRITING	ENDS		**********************
	//////////////////////////////////////////////////////////////////////////
	// ******************* 		THE REST BEGINS			**********************

	// **********************************************************
	// This is the engine that calls the rest member functions	*
	// to build the response. 									*
	// **********************************************************
	void Response::buildResponse( void )
	{
		std::ostringstream	tmp;
		ErrorCode			status = NONE;

		if ( _request->readStatusCode() == NONE )
		{
			//
			if (isCgiFile(this->_request->readPath()))
			{
				Cgi cgi_request(this->_server);
				cgi_request.set_request(this->_request);
				cgi_request.run_cgi();
				cgi_request.parse_body_for_headers();
				status = cgi_request.getErrorCode();
				this->_web_page = cgi_request._request->getRequestBody();
				std::string("").swap(cgi_request._request->getRequestBody());
			}
			else if ( _request->readMethod() == GET )
					status = doGetPost("GET");
			else if ( _request->readMethod() == POST )
				status = doGetPost("POST");
			else if ( _request->readMethod() == HEAD )	// Same as GET except it doesn't return a Body;
				status = doGetPost("HEAD");
			else if ( _request->readMethod() == PUT )	// Same as POST except it doesn't return a Body;
				status = doGetPost("PUT");
			else if ( _request->readMethod() == DELETE )
				status = doDelete();
			else
				status = METHODNOTALLOWED;
		}
		else if ( _request->readStatusCode() != NONE )
			status = _request->readStatusCode();	// we retrieve status code set from httprequest

		if ( status != OK && status != MOVEDPERMANENTLY && status != FOUND )
			buildErrorCodePage(_web_page, status);

		// Add httpResponse status line to stream z.B [HTTP/1.1 200 OK]
		// Add content-type to stream z.B [Content-Type: text/html]
		tmp << _request->readProtocol() << " "
			<< status << " "
			<< ft::translateErrorCode(status) << EOL
			<< "Content-Type: " << getContentType(_loc_file_path, status) << EOL
			<< "Content-Length: " << _web_page.size() << EOL
			<< "Server: " << "webserv/1.0" << EOL
			<< "Date: " << http::get_http_date() << EOL
			<< "Location: " << _location
			<< "\n\n"
			<< (( _request->readMethod() != HEAD && _request->readMethod() != PUT ) ? _web_page : "");
		// clear() _response_content before assigning it the new response
		std::string("").swap(_response_content);
		_response_content = tmp.str();
		// clear() all other data 
		_request->clear();
		std::string("").swap(_loc_file_path);
		std::string("").swap(_web_page);
		std::string("").swap(_location);
		std::string("").swap(_root_directory);
	}
	void Response::set_bytesend(int n)
	{
		this->byte_send = n;
	}
	int Response::get_bytesend(void)
	{
		return this->byte_send;
	}

	// ******************************************************
	// This function is called from buildResponse() when	*
	// the client request uses a DELETE method				*
	// ******************************************************
	ErrorCode	Response::doDelete( void ) {
		std::string			delete_me;			// stores the website URL path requested by client z.B "/directory/index.html"
		std::string			delete_me_dir;		// directory copied from delete_me z.B. "/directory/"
		std::string			delete_me_fname;	// filename copied from delete_me z.B. "index.html"
		ErrorCode			status = NONE;

		delete_me = _request->readPath();
		if ( (status = extractDirFromWebUrl(delete_me_dir, delete_me_fname, delete_me)) != NONE )
			return status;

		std::vector<http::Location>::iterator	it = _server->refLocations().begin();
		if ( (status = setIteratorToLocationContext(it, delete_me_dir, delete_me_fname, "DELETE")) != NONE )
			return status;

		// Delete directory or file
		if ( ft::isDirectory(_loc_file_path) )
			ft::deleteDirectory(_loc_file_path);
		else
			std::remove(_loc_file_path.c_str());

		// After deleting resource, Redirect client back to referrer or home
		_location = _request->readHeaders().count("referer") > 0 ? _request->readHeaders().find("referer")->second : "/";

		return OK;
	}

	// ******************************************************************************
	// handles the GET/POST request. It uses the Location context (of the Server	*
	// that received request from incoming client) to calculate the appropriate		*
	// httpResponse status code. Then if the status code is 200(OK), it retrieves	*
	// the webpage file from local filepath (depending on the GET path of 			*
	// the client's httpRequest data). 												*
	// Return value is the status code of the processed httpRequest					*
	// ******************************************************************************
	ErrorCode	Response::doGetPost( const char *method ) {
		std::string		web_url;			// stores the website URL path requested by client z.B "/directory/index.html"
		std::string		web_url_dir;		// directory copied from web_url z.B. "/directory/"
		std::string		web_url_fname;		// filename copied from web_url z.B. "index.html"
		ErrorCode		status = NONE;	

		// copy directory part of web_url to web_url_dir
		// and filename part of web_url to web_url_fname
		web_url = _request->readPath();
		if ( (status = extractDirFromWebUrl(web_url_dir, web_url_fname, web_url)) != NONE )
			return status;

		// set iterator it to location context of config that will route this request
		std::vector<http::Location>::iterator	it = _server->refLocations().begin();
		if ( (status = setIteratorToLocationContext(it, web_url_dir, web_url_fname, method)) != NONE ) // we need to send you file_name
			return status;
		// Check If Location context has max_body value set. If yes, compare accordingly
		if ( it->readMaxBody() && it->readMaxBody() < _request->getRequestBody().size() )
			return CONTENTTOOLARGE;

		// Saving data/body received from requests, if any
		std::ofstream 		_fout;
		std::ostringstream	buff_tmp;
		buff_tmp << (it->readUploads().size() > 0 ? it->readUploads() : "queryData") << "/"
			<< ( _request->getRequestBody().size() > 0 ? "postQery" : _request->readQuery().size() > 0 ? "getQery" : "tmp");
		_fout.open(buff_tmp.str().c_str(), std::ios::out | std::ios::app );
		if (! _fout.good() )
			print_status(ft_RED, "Skipping GET/POST/PUT Query Backup Because Uploads Path Not Created");
		else {
			if ( strcmp(method, "POST") == 0 || strcmp(method, "PUT") == 0 )				// it's a post or put request
				collatePostQuery(_request->getRequestBody(), _fout, it->readUploads());
			else if ( _request->readQuery().size() > 0 )				// it's a get request that has query parameters			
				_fout << _request->readQuery() << std::endl;
			_fout.close();
		}

		// check if any redirection is present
		if ( strcmp(method, "GET") == 0 || strcmp(method, "HEAD") == 0 )
		{
			if ( (status = checkForRedirections(_loc_file_path, web_url, it)) != NONE )
				return status;
		}


		// Open an input file stream, write to stream from _loc_file_path,
		// if write failed, return error404, else read from stream to 
		// _web_page and return status 200
		if (this->_request->readMethod() == GET || this->_request->readMethod() == HEAD)
		{
			std::ifstream		fin;
			fin.open(_loc_file_path.c_str());
			if ( ! fin.good() )
				return NOTFOUND;
			std::stringstream buff_tmp;
			buff_tmp << fin.rdbuf();
			fin.close();
			_web_page.append(buff_tmp.str().c_str(), buff_tmp.str().length());
		}
		return OK;
	}

	// ******************************************************************
	// request_path(param3) is the web URL path requested by Client.	*
	// request_fname(param2) is an outParam where we store only the		*
	// filename retrieved from request_path(param3) /if any filename).	*
	// request_dir(param1) is an outParam where we store only the		*
	// directory part retrieved from request_path(param3).				*
	// Return value is the appropriate httpResponseCode or NONE if 		*
	// everything went well.											*
	// ******************************************************************
	ErrorCode	Response::extractDirFromWebUrl( std::string& request_dir,
										std::string& request_fname, std::string& request_path ) {
		std::size_t		pos;

		// erase spaces at the beginning (if any)
		pos = request_path.find_first_not_of(' ');
		if ( pos )
			request_path.erase(0, pos);
		
		if ( request_path.size() <= 0 || (request_path.size() > 0 && request_path.at(0) != '/') )
			return BADREQUEST;

		// copy only directory part from request_path to request_dir
		// then copy only filename part from request_path to request_fname
		request_dir = request_path;
		if ( (pos = request_dir.rfind('.')) != std::string::npos ) {
			if ( (pos = request_dir.rfind('/', pos)) != std::string::npos )
				request_dir = request_dir.substr(0, pos + 1);
			if ( pos == std::string::npos || request_path.at(request_dir.size() + 1) == '.' )	// we dont want "www.google.com/."
				return BADREQUEST;

			request_fname = request_path.substr(pos + 1);
			if ( (pos = request_fname.find_last_of('.')) != std::string::npos )
				if ( ! types::parseMime( request_fname.substr(pos) ).compare("default") )		// has bad extension
					return BADREQUEST;
		}
		return NONE;
	}

	// **************************************************************
	// Searches through the Location contexts to see if path has	*
	// its own specific Location context set. If none found, we		*
	// then search for the Location context'/'						*
	// it(param1) is == _server->refLocations().begin(),			*
	// path(param2) is a string containing the Location context 	*
	// path that we're searching for,								*
	// fname(param3) is a string containing only the file_name that	*
	// the client requested for (minus the directory path)			*
	// method(param4) is the method that the Location matching		*
	// our path must support.										*
	// **************************************************************
	ErrorCode	Response::setIteratorToLocationContext( std::vector<http::Location>::iterator& it,
									const std::string& path, std::string& fname, const char *method ) {
		std::string		search_key = path;
		std::string		reserved;			// used for storing the temp chopped-off directory while loop is running
		bool			flag = false;
		std::size_t		pos;

		pos = search_key.size();
		if ( pos && search_key.at(pos - 1) != '/' )
			search_key.push_back('/');
		while ( it != _server->refLocations().end() ) {
			if ( ! it->readPath().compare(search_key) && (std::find(it->refMethods().begin(),
						it->refMethods().end(), method) != it->refMethods().end()) ) {
				_loc_file_path = it->readRoot();
				_root_directory = it->readRoot();
				_loc_file_path.append(reserved);
				_loc_file_path.append(fname);
				flag = true;
				break ;
			}
			++it;
			if ( it == _server->refLocations().end() && search_key.size() > 1 ) { // ">1" explanation: if search_key.size() == 1 at this point, it means search_key has used its final chance which is '/' but still failed to find a supported location context
				if ( (pos = search_key.rfind('/', search_key.size() - 2)) != std::string::npos ) {
					reserved.insert( 0, search_key.substr(pos + 1) );
					search_key.erase(pos + 1);
					it = _server->refLocations().begin();
				}
			}
		}
		if ( flag == false  )
			return METHODNOTALLOWED;
		return NONE;
	}

	// **************************************************************************
	// Collate query parameters of POST requests and store uploaded files.		*
	// post_query param is a string that holds the content of POST body.		*
	// _fout param is an outstream already connected to the file where we're	*
	// storing the post queries.												*
	// uploads_dir param is a string that holds the uploads directory assigned	*
	// for the route serving this request in config file.						*
	// **************************************************************************
	void	Response::collatePostQuery( const std::string& post_query,
						std::ofstream& _fout, const std::string& uploads_dir ) {
		std::size_t		pos = 0;
		std::string		file_name;
		std::string		tmp;

		// WebKitFormXXX might be peculiar to only POST queries
		// sent from a Chrome Browser
		if (this->_request->headers.count("content-type") && this->_request->headers["content-type"].find("multipart/form-data") != std::string::npos)
		{
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

						// tmp.insert(0, 1, '/');
						tmp.insert(0, (uploads_dir.size() > 0 ? uploads_dir.c_str() : "queryData/") );
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
					else if ( ! post_query.compare(pos, 4,"\r\n\r\n") ) {
						pos += std::strlen("\r\n\r\n");
						_fout << post_query.substr(pos, post_query.find("------WebKitForm", pos) - pos);
					}
				}
			}
		}
		else
		{
			std::ofstream 	_uploaded_file;
			std::string file_name = uploads_dir + "/" + _request->readPath().substr(_request->readPath().rfind('/') + 1); 
			http::remove_extra_backslash(file_name);
			_uploaded_file.open(file_name.c_str(), std::ios::out | std::ios::trunc );
			if ( _uploaded_file.good() ) {
				_uploaded_file << this->_request->getRequestBody();
			}
			_uploaded_file.close();
		}
		_fout.close();
	}

	// **************************************************************
	// used for building the "Content-Type: " part of httpresponse	*
	// Body. Return Value is the appropriate browser compatible 	*
	// content-type.												*
	// loc_file_path(param1) is the filepath to the file we're 		*
	// sending to client.
	// status(param2) is the holds the responsecode value that 		*
	// we're sending to client.
	// **************************************************************
	std::string	Response::getContentType( const std::string& loc_file_path, const ErrorCode& status ) {
		std::string		tmp;
		std::size_t		pos;

		// Checking if we are returning an error page to 
		if ( !(status == OK || status == MOVEDPERMANENTLY || status == FOUND ))  {
			tmp.insert( 0, "text/html" );
			return tmp;
		}

		// Getting file extension stored to tmp
		if ( (pos = loc_file_path.find_last_of('.')) != std::string::npos )
			tmp = loc_file_path.substr(pos);

		// retrieving the necessary browser-compatible content-type description
		tmp = types::parseMime(tmp);
		return tmp;
	}

	// **********************************************************************
	// Use supported error code provided by status(param2) to build their 	*
	// corresponding webpages.												*
	// Just name the error code file the "ERROR_NUMBER.html" and put it in	*
	// the _server->readErrorPage() directory. 								*
	// If _server->readErrorPage().size() == 0, (error_page wasn't set in 	*
	// config file), then we use "public_html/error_pages/" automatically	*
	// **********************************************************************
	void	Response::buildErrorCodePage(std::string& web_page, ErrorCode& status) {
		std::ostringstream	buff_tmp;
		std::ifstream		fin;
		std::string			error_pages;

		// if we have a LISTDIRECTORYCONTENT request (AKA autoindex	on), do 
		if ( status == LISTDIRECTORYCONTENTS ) {
			ft::listDirectoryContent(web_page, _loc_file_path, _root_directory);
			status = OK;
			return;
		}

		// assign the error_page value from server config to error_pages,
		// if error_pages.size() <= 0, use our default error page directory
		// else, use the error_page value from server config
		error_pages = _server->readErrorPage();
		if ( error_pages.size() == 0 )
			buff_tmp << "public_html/error_pages/" << status << ".html";
		else
			buff_tmp << error_pages.c_str() << status << ".html";

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
	ErrorCode	Response::checkForRedirections(std::string& loc_file_path,
				std::string& web_url_path, std::vector<http::Location>::iterator& it) {
		std::size_t		pos;

		// we need the loc_file_path to include file name extension if any
		// loc_file_path == public_html/
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
				if ( ! tmp_local_path.empty() && *(tmp_local_path.end() - 1) != '/' )
					tmp_local_path.push_back('/');
				tmp_local_path.append( ((it->readIndex().size() > 0) ? it->readIndex() : ";)") ); // append ;)?
				fin.open(tmp_local_path.c_str());
				if ( ! fin.good() ) {
					_location.clear();
					
					// if autoindex is "on", directories that do not have an index.html file will 
					// will list their contents intead of showing "403 forbidden" error
					if ( ! it->readAutoind().compare("on") ) {
						return LISTDIRECTORYCONTENTS;
					}
					
					return NOTFOUND;
				}
			}
			if (*(_location.end() - 1) == '/')
				_location.append(it->readIndex());
			else
				_location.append("/" + it->readIndex());
			return MOVEDPERMANENTLY;	
		}

		// Check if 302 (AKA http redirect) redirection is present in route/location
		else if ( it->readRewrite().size() ) {
			_location.append(it->readRewrite().c_str());
			return FOUND;
		}
		return NONE;
	}

	bool Response::isCgiFile(const std::string& file)
	{
		if (this->_server->getCgi().empty() || file.find_last_of(".") == std::string::npos)
			return false;
		std::string ext = file.substr(file.find_last_of("."));
		std::map<std::string, std::pair<std::string, std::string> >::iterator element = this->_server->getCgi().find(ext);
		if (element == this->_server->getCgi().end())
			return false;
		if (element->second.first == "GET" && this->_request->readMethod() != GET )
			return false;
		if (element->second.first == "POST" && this->_request->readMethod() != POST )
			return false;
		else
		{
			this->_request->setCgi_exe(element->second.second);
			this->_request->setCgi_method(element->second.first);
			return true;
		}
	}

}	// namespace http
