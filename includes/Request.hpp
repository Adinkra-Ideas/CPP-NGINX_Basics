#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "WebServer.hpp"
# include "utils.hpp"
# include "Types.hpp"
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
	DELETE,
	HEAD,
	PUT
};

enum Chunk
{
	CHUNKSIZE,
	CHUNKDATA
};

// Class that stores the httpRequest data of a client
class Request
{
	public:
		typedef	typename std::map<std::string, std::string>	headers_map_obj;

		// Constructors
		Request();
		Request(const Request &copy);
		
		// Destructor
		~Request();
		
		// Operators
		Request & operator=(const Request &assign);
		void parse(std::string &buffer);
		bool parsingFinished();
		std::string getServerName();
		ErrorCode getErrorCode();
		void clear();

		const std::string&		readProtocol( void );
		const std::string&		readPath( void );
		const std::string&		readQuery( void );
		const Method&			readMethod( void );
		std::string 			getRequestBody();
		bool 					keepAlive();
		const ErrorCode&		readStatusCode( void );
		void					setStatusCode( const ErrorCode error );
		void 					setCgi_exe(std::string str);
		std::string				getCgi_exe();
		void setCgi_method(std::string str);
		std::string	getCgi_method();
		const headers_map_obj&	readHeaders( void );
		void set_max_body_size(size_t n);
		bool has_request();
		

	//private:
		void first_line();
		void parse_headers();
		void prepare_for_body();
		void parse_body();
		void parse_chunks();
		void parseMethod(std::string str);
		void parsePath(std::string str);
		void parseProtocol(std::string str);
		size_t parse_str_to_int(std::string str);
		bool not_allowed_char_in_URL();
		bool not_allowed_char_in_field(std::string value);
		bool not_allowed_char_in_key(std::string value);
		void trailing_chunk();
		Status parse_status;
		std::string buffer;
		Method method;				//what kind of request: GET,POST, DELETE ...
		std::string serverName;		//Name of the Server
		std::string path;			// path of the target of the request
		std::string query;			// query of the request
		std::string protocol;		//protocol of the request, HTTP 1.1 for us
		std::string request_body;	// the body part of the request
	public:
		std::map<std::string, std::string> headers;	//place to story everything of the header, like Host, Chunk sending and so on
	private:
		size_t	chunk_length; //length of the chunks
		size_t	body_length; //length of the body
		size_t	length;
		struct timeval start_timer; //for timeout checking
  		struct timeval last_timer;
		ErrorCode error_code; //enum of the error codes
		Chunk chunk_part;
		std::string body;
		bool keep_alive;
		std::string cgi_exe;
		std::string cgi_method;
		size_t max_body_size;
		bool request_started;
};

#endif