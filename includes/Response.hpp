#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

# include <iostream>
# include <algorithm>
# include <sstream>
# include <fstream>
# include <string>
# include <cstring>
# include <vector>
# include <unistd.h>
# include <cstdio>
# include <filesystem>
# include "Request.hpp"
# include "Server.hpp"
# include "Location.hpp"
# include "utils.hpp"
# include "Mime.hpp"
# include "Cgi.hpp"

namespace http {

	class Response {
	public:
		// Constructors
		Response();
		Response(const Response &copy);
		
		// Destructor
		~Response();
		
		// Operators
		Response & operator=(const Response &assign);

		void        	setRequest(Request &request);
		void        	setServer(http::Server &server);
		void			buildResponse( void );
		std::string&	refResponseCont( void );

	private:
		Request				_request;			// object holding the parsed requestheader
		http::Server		*_server;			// pointer to server object managing this response
		std::string			_loc_file_path;		// The final local/server filepath after concatening the root path from config file with the httpRequest path received from client
		std::string			_web_page;				// where the returned webpage will be stored
		std::string     	_response_content;		// where the http response will be stored
		std::string			_location;				// Web address where the client's data eventually got fetched from (after all the redirections)
		std::string			_root_directory;		// directory used as root by the root(aka location context) servicing this request from _server config
		// std::string			_key_value;			// if a GET request has key=>values appended, they will be backed up to this string

		// private functions/methods
		std::string			translateErrorCode( const ErrorCode& status_code );
		std::string			getContentType( const std::string& loc_file_path, const ErrorCode& status );
		void				buildErrorCodePage(std::string& web_page, ErrorCode& status);
		ErrorCode			doGetPost( std::string& loc_file_path, const char *method );
		ErrorCode			doDelete();
		ErrorCode			checkForRedirections(std::string& loc_file_path,
								std::string& web_url_path, std::vector<http::Location>::iterator& it);
		bool 				isCgiFile(const std::string& file);
		void				collatePostQuery( const std::string& post_query,
								std::ofstream& _fout, const std::string& uploads_dir  );
	};

}	// namespace http

#endif
