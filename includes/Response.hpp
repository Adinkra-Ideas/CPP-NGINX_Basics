#ifndef RESPONCE_HPP
# define RESPONCE_HPP

# include <iostream>
# include <string>
# include "Request.hpp"
# include "ServerConfig.hpp"

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
        void        setServer(ServerConfig &server);
		void		buildResponse();
		std::string     response_content;
	private:
		Request			request;
		ServerConfig	server;
		
};

#endif