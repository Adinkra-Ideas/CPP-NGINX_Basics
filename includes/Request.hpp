#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>

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
	POST
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

	private:
		int first_line();
		int parse_headers();
		int prepare_for_body();
		int parse_body();
		int parse_chunks();

		Status parse_status;
		std::string buffer;
		Method method;
};

#endif