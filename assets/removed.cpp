// from Response::respondGetRequest
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



	// ******************************************************************************
	// handles the GET method request. It uses the Location context (of the Server	*
	// that received request from incoming client) to calculate the appropriate		*
	// httpResponse status code. Then if the status code is 200(OK), it retrieves	*
	// the webpage file from local filepath (depending on the GET path of 			*
	// the client's httpRequest data). 												*
	// Return value is the status code of the processed httpRequest					*
	// ******************************************************************************
	ErrorCode	Response::respondGetRequest( std::string& loc_file_path) {
		std::string		dir_tmp;		// z.B. /public/index.html (path retrieved from client'S request header)
		std::string		web_url_path;		// stores the website child page requested by client without the '/' at the beginning
		std::size_t		pos;

		dir_tmp = _request.readPath();

		// erase spaces at the beginning (if any)
		pos = dir_tmp.find_first_not_of(' ');
		if ( pos )
			dir_tmp.erase(0, pos);

		// In the client's http request, split the directory from path. 
		// Store directory to dir_tmp and filename to web_url_path
		if ( dir_tmp.size() > 0) {
			pos = dir_tmp.find_first_of('/');
			web_url_path = dir_tmp.substr(pos + 1);
			dir_tmp.erase(pos + 1);
		} else {
			return NOTFOUND;
		}

		// checking if dir_tmp is an existing Location context in _server
		std::vector<http::Location>::iterator it = _server->refLocations().begin();
		for ( ; it != _server->refLocations().end(); ++it )
			if ( ! it->readPath().compare(dir_tmp) )
				break ;

		// building the _loc_file_path path for client's request. But first
		// we check if dir_tmp wasn't found among Location context in _server
		if ( it == _server->refLocations().end() )
			return NOTFOUND;
		else {
			loc_file_path.append(it->readRoot());
			loc_file_path.append(dir_tmp);
			loc_file_path.append(web_url_path);
		}

		// ErrorCode	status = NONE;
		// if ( ( status = check_for_redirection() != NONE )
		// 	return status; 

		// ErrorCode	check_for_redirections(std::string& loc_file_path, )

		
		// Check if 301 redirection is necessary for Client's requested path
		// If yes, set status to MOVEDPERMANENTLY and store new Web URL to _location
		if ( ft::isDirectory(loc_file_path) ) {
			pos = web_url_path.size();
			if ( pos > 0 && web_url_path.at(pos - 1) != '/' )
				web_url_path.push_back('/');
			_location.append(dir_tmp);
			_location.append(web_url_path);

			// // if it's a dir and no index file found, or index file not readable
			{
				std::ifstream		fin;
				std::string			tmp_local_path = loc_file_path;

				if ( ! tmp_local_path.empty() && *tmp_local_path.end() - 1 != '/' )
					tmp_local_path.push_back('/');
				tmp_local_path.append(it->readIndex());
				fin.open(tmp_local_path.c_str());
				if ( ! fin.good() ) {
					_location.clear();
					return FORBIDDEN;
				}
			}

			_location.append(it->readIndex());
			return MOVEDPERMANENTLY;	
		}

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
			_web_page.insert(0, buff_tmp.str().c_str());
		}
		return OK;
	}