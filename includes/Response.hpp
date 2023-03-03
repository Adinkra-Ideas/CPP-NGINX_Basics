#ifndef RESPONCE_HPP
# define RESPONCE_HPP

# include <iostream>
# include <string>
# include "Request.hpp"
# include "Server.hpp"

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
        void        setServer(ft::Server &server);
		void		buildResponse();
		std::string     response_content;
	private:
		Request			request;
		ft::Server	server;
		
};

#endif