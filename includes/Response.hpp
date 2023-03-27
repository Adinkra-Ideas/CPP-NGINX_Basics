#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

# include <iostream>
# include <algorithm>
# include <sstream>
# include <fstream>
# include <string>
# include <cstring>
# include <vector>
# include "Request.hpp"
# include "Server.hpp"
# include "Location.hpp"
# include "utils.hpp"
# include "Mime.hpp"

namespace http {

	class Response
	{
		public:
			// Constructors
			Response();
			Response(const Response &copy);
			
			// Destructor
			~Response();
			
			// Operators
			Response & operator=(const Response &assign);

			void        setRequest(Request &request);
			void        setServer(http::Server &server);
			void		buildResponse( void );

			std::string& refResponseCont( void );

		private:
			Request				_request;			// object holding the parsed requestheader
			http::Server		*_server;			// pointer to server object managing this response
			std::string			_loc_file_path;		// The local/server filepath of the final file to be delivered to the client will be stored here. Including the file extension, coz the extension will be required when building the "content_type" part of the header response
			std::string			_web_page;				// where the returned webpage will be stored
			std::string     	_response_content;		// where the http response will be stored
			std::string			_location;				// Web address where the client's data eventually got fetched from 
			std::string			_root_directory;		// directory used as root by the root(aka location context) servicing this request from _server config
			std::string			_key_value;			// if a GET request has key=>values appended, they will be backed up to this string

			// private functions/methods
			std::string			translateErrorCode( const ErrorCode& status_code );
			std::string			getContentType( const std::string& loc_file_path, const ErrorCode& status );
			void				buildErrorCodePage(std::string& web_page, ErrorCode& status);
			ErrorCode			doGetPost( std::string& loc_file_path, const char *method );
			ErrorCode			check_for_redirections(std::string& loc_file_path,
								std::string& web_url_path, std::vector<http::Location>::iterator& it);
	};

}	// namespace http

#endif
