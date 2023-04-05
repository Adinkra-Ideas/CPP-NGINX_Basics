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

	void Response::buildResponse( void )
	{
		std::ostringstream	tmp;
		ErrorCode			status = NONE;

		// parseUrl( _request.readPath() );	// for removing the %%%% and other unformated chars from URL

		if ( _request.readProtocol().compare("HTTP/1.1") )
			status = HTTPVERSIONNOTSUPPORTED;
		else if ( _request.readStatusCode() == NONE )
		{
			//TODO very simple way of checking if cgi just checking if cgi-bin is in path
			if (isCgiFile(this->_request.readPath()) && _request.readMethod() != DELETE)
			{
				Cgi cgi_request(this->_request);
				status = cgi_request.getErrorCode();
				this->_web_page = cgi_request.getBody();
			}
			else if ( _request.readMethod() == GET )
					status = doGetPost("GET");
			else if ( _request.readMethod() == POST )
				status = doGetPost("POST");
			else if ( _request.readMethod() == HEAD )
				status = doGetPost("HEAD");
			else if ( _request.readMethod() == PUT )
				status = doGetPost("PUT");
			else if ( _request.readMethod() == DELETE )
				status = doDelete();
			else
				status = METHODNOTALLOWED;
		}
		else if ( _request.readStatusCode() != NONE )
			status = _request.readStatusCode();	// we retrieve status code set from httprequest

		if ( status != OK && status != MOVEDPERMANENTLY && status != FOUND )
			buildErrorCodePage(_web_page, status);

		// Add httpResponse status line to stream z.B [HTTP/1.1 200 OK]
		// Add content-type to stream z.B [Content-Type: text/html]
		tmp << _request.readProtocol() << " "
			<< status << " "
			<< ft::translateErrorCode(status) << "\n"
			<< "Content-Type: " << getContentType(_loc_file_path, status) << " \n"
			<< "Content-Length: " << _web_page.size() << " \n"
			<< "Location: " << (( _request.readMethod() != HEAD && _request.readMethod() != PUT ) ? _location : "")
			<< "\n\n"
			<< (( _request.readMethod() != HEAD && _request.readMethod() != PUT ) ? _web_page.c_str() : "");

		_response_content.clear();
		_response_content = tmp.str();

		_request.clear();
		_loc_file_path.clear();
		_web_page.clear();
		_location.clear();
		_root_directory.clear();
	}

	ErrorCode	Response::doDelete() {
		std::string			delete_me;			// stores the website URL path requested by client z.B "/directory/index.html"
		std::string			delete_me_dir;		// directory copied from delete_me z.B. "/directory"
		std::string			delete_me_fname;	// filename copied from delete_me z.B. "/index.html"
		ErrorCode			status = NONE;

		delete_me = _request.readPath();
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
		_location = _request.readHeaders().count("referer") > 0 ? _request.readHeaders().find("referer")->second : "/";

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
		std::string		web_url_dir;		// directory copied from web_url z.B. "/directory"
		std::string		web_url_fname;		// filename copied from web_url z.B. "/index.html"
		ErrorCode		status = NONE;	

		// copy directory part of web_url to web_url_dir
		// and filename part of web_url to web_url_fname
		web_url = _request.readPath();
		if ( (status = extractDirFromWebUrl(web_url_dir, web_url_fname, web_url)) != NONE )
			return status;

		// set it to location context of config that will route this request
		std::vector<http::Location>::iterator	it = _server->refLocations().begin();
		if ( (status = setIteratorToLocationContext(it, web_url_dir, web_url_fname, method)) != NONE ) // we need to send you file_name
			return status;

		// backingup data received from GET/POST requests, if any
		std::ofstream 		_fout;
		std::ostringstream	buff_tmp;
		buff_tmp << (it->readUploads().size() > 0 ? it->readUploads() : "queryData") << "/"
			<< ( _request.getRequestBody().size() > 0 ? "postQery" : _request.readQuery().size() > 0 ? "getQery" : "");
		_fout.open(buff_tmp.str().c_str(), std::ios::out | std::ios::app );
		if (! _fout.good() )
			print_status(ft_RED, "Skipping GET/POST/PUT Query Backup Because Uploads Path Not Created");
		else {
			if ( _request.getRequestBody().size() > 0 )				// it's a post or put request
				collatePostQuery(_request.getRequestBody(), _fout, it->readUploads());
			else if ( _request.readQuery().size() > 0 )				// it's a get request that has query parameters			
				_fout << _request.readQuery() << std::endl;
			_fout.close();
		}

		// check if any redirection is present
		// if ( _request.readMethod() != HEAD )
			if ( (status = checkForRedirections(_loc_file_path, web_url, it)) != NONE )
				return status;

		// Open an input file stream, write to stream from _loc_file_path,
		// if write failed, return error404, else read from stream to 
		// _web_page and return status 200
		{
			std::ifstream		fin;
			fin.open(_loc_file_path.c_str());
			if ( ! fin.good() )
				return NOTFOUND;
			std::stringstream buff_tmp;
			buff_tmp << fin.rdbuf();
			fin.close();
			// It is at this point that PHP or other backend scripters are exec'd on the file
			_web_page.append(buff_tmp.str().c_str());
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
		
		if ( request_path.size() <= 0 )
			return NOTFOUND;

		// put '/' at the beginning if absent
		if ( request_path.size() > 0 && request_path.at(0) != '/' )
			request_path.insert(0, 1, '/');

		pos = request_path.size();
		if ( pos > 2 && request_path.at(pos - 1) == '/' )	// >2 meaning at least std::strlen("/x/")
			request_path.erase(pos - 1, 1);	// we want to search Location context using z.B '/web_url_path' not '/web_url_path/'
		
		// copy only directory path from request_path to request_dir
		request_dir = request_path;
		if ( (pos = request_dir.find('.')) != std::string::npos ) {
			if ( (pos = request_dir.rfind('/', pos)) != std::string::npos )
				request_dir = request_dir.substr(0, pos);
			if ( pos == std::string::npos || request_path.at(request_dir.size() + 1) == '.' ) // we dont want "www.google.com/."
				return BADREQUEST;
			request_fname = request_path.substr(pos);
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
	// method(param3) is the method that the Location matching		*
	// our path must support.										*
	// **************************************************************
	ErrorCode	Response::setIteratorToLocationContext( std::vector<http::Location>::iterator& it,
										std::string& path, std::string& fname, const char *method ) {
		std::string		*ptr = &path;
		std::string		dir_sign = "/";
		bool			flag = false;

		while ( it != _server->refLocations().end() ) {
			if ( ! it->readPath().compare(*ptr) && (std::find(it->refMethods().begin(),
						it->refMethods().end(), method) != it->refMethods().end()) ) {
				_loc_file_path = it->readRoot();
				_root_directory = it->readRoot();
				if ( flag )		// meaning we wanted a Location "/path" but ended up settling for Location "/". Therefore, '/path' needs to be appended to the path that '/' is rooted to
					_loc_file_path.append(path);
				_loc_file_path.append(fname);
				break ;
			}
			++it;
			if ( it == _server->refLocations().end() && !flag ) {
				flag = true;
				it = _server->refLocations().begin();
				ptr = &dir_sign;
			}
		}
		if ( it == _server->refLocations().end() )
			return METHODNOTALLOWED;
		return NONE;
	}

	// **************************************************************************
	// collate query parameters of POST requests and store uploaded files.		*
	// post_query param is a string that holds the content of POST body.		*
	// _fout param is an outstream already connected to the file where we		*
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

					tmp.insert(0, 1, '/');
					tmp.insert(0, (uploads_dir.size() > 0 ? uploads_dir.c_str() : "queryData") );
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
					pos += std::strlen("\r\n\r\n");
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

	bool Response::isCgiFile(const std::string& file)
	{
		if (this->_server->getCgi().empty() || file.find_last_of(".") == std::string::npos)
			return false;
		std::map<std::string, std::vector<std::string> >::const_iterator it = this->_server->getCgi().begin();
		std::map<std::string, std::vector<std::string> >::const_iterator itend = this->_server->getCgi().end();
		std::string ext = file.substr(file.find_last_of("."));
		
		while (it != itend)
		{
			if (std::find(it->second.begin(), it->second.end(), ext) != it->second.end())
			{
				this->_request.setCgi_exe(it->first);
				return true;
			}
			it++;
		}
		return false;
	}

}	// namespace http
