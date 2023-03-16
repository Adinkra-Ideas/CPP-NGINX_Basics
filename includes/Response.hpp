#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include "Request.hpp"
# include "Server.hpp"
# include "Location.hpp"
# include "utils.hpp"

// We will assemble all the required httpResponse status codes here
// enum statusCode
// {
	// OK = 200,				// Already present in Request.hpp
	// 301
	// 302
	// 400
	// UNAUTHORIZED = 401,		// Already present in Request.hpp
	// 402
	// FORBIDDEN = 403,			// Already present in Request.hpp
	// NOTFOUND = 404,			// Already present in Request.hpp
	// METHODNOTALLOWED = 405,	// Already present in Request.hpp
	// 406
	// 500
	// NOTIMPLEMENTED = 501		// Already present in Request.hpp
	// 502
	// 503
	// 505
	// 505
// };

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
		Request				_request;	// object holding the parsed requestheader
		http::Server		*_server;	// pointer to server object 

		std::string			_requested_file;		// The filepath of the final file to be delivered to the client will be stored here. Including the file extension, coz the extension will be required when building the "content_type" part of the header response
		std::string			_web_page;				// where the returned webpage will be stored
		std::string     	_response_content;		// where the http response will be stored

		std::string	translateErrorCode( const ErrorCode& status_code );
		std::string	getContentType( const std::string& requested_file, const ErrorCode& status );
		const std::string&	buildErrorCodePage(std::string& web_page, const ErrorCode& status);
		ErrorCode	processGetRequest( std::string& requested_file );
};

#endif
