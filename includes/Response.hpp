#ifndef RESPONCE_HPP
# define RESPONCE_HPP

# include <iostream>
# include <sstream>
# include <string>
# include "Request.hpp"
# include "Server.hpp"
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
		void		buildResponse();
		std::string     response_content;

	private:
		Request			request;	// object holding the parsed requestheader
		http::Server	server;		// what does this server object is this?

		std::string			_requested_file;		// The filepath of the final file to be delivered to the client will be stored here. Including the file extension, coz the extension will be required when building the "content_type" part of the header response



		std::string	translateErrorCode( const ErrorCode& status_code );
		std::string	getContentType( const std::string& requested_file );
		ErrorCode	processGetRequest( std::string& requested_file );
};

#endif