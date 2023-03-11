#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "WebServer.hpp"
# include <iostream>
# include <string>
# include <map>

enum Status
{
	FIRST_LINE,
	HEADERS,
	PREBODY,
	BODY,
	CHUNK,
	COMPLETED,
	ERROR
};

enum Method
{
	GET,
	POST,
	DELETE
};

enum ErrorCode
{
	NONE,
	OK = 200,
	BADREQUEST = 400,
	UNAUTHORIZED = 401,
	FORBIDDEN = 403,
	NOTFOUND = 404,
	METHODNOTALLOWED = 405,
	NOTIMPLEMENTED = 501
};

class Request
{
	public:
		// Constructors
		Request();
		Request(const Request &copy);
		
		// Destructor
		~Request();
		
		// Operators
		Request & operator=(const Request &assign);
		int parse(std::string &buffer);
		bool parsingFinished();
		std::string getServerName();
		ErrorCode getErrorCode();
		void clear();
	//private:
		int first_line();
		int parse_headers();
		int prepare_for_body();
		int parse_body();
		int parse_chunks();
		void parseMethod(std::string str);
		void parsePath(std::string str);
		void parseProtocol(std::string str);
		Status parse_status;
		std::string buffer;
		Method method;				//what kind of request: GET,POST, DELETE ...
		std::string serverName;		//Name of the Server
		std::string path;			// path of the target of the request
		std::string query;			// query of the request
		std::string protocol;		//protocol of the request, HTTP 1.1 for us
		std::string request_body;	// the body part of the request
		std::map<std::string, std::string> headers;	//place to story everything of the header, like Host, Chunk sending and so on
		size_t	chunk_length; //length of the chunks
		size_t	body_length; //length of the body
		size_t	length;
		struct timeval start_timer; //for timeout checking
  		struct timeval last_timer;
		ErrorCode error_code; //enum of the error codes
};

#endif