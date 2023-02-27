#pragma once
#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# define ft_RESET_COLOR	"\033[0m"
# define ft_RED			"\033[1;31m"
# define ft_YELLOW		"\033[1;33m"

# include <iostream>
# include <string>
# include <vector>
# include <cstdlib>
# include <fstream>

# include <sys/socket.h>
# include <arpa/inet.h>

# include "Location.hpp"

namespace ft {

	class Server {
	public:
		Server( void );
		~Server( void );
		Server( const Server& other );

		Server& operator= ( const Server& other );

	private:
		struct	sockaddr_in		_socketAddress;			// (MANDATORY) Choose the port and host of each ’server’
		std::string				_server_name;			// (OPTIONAL)	Set the server_names or not
		std::string				_root;					// (MANDATORY)	Define a directory or a file from where the returned file should be searched
		std::size_t				_client_max_body_size;	// (OPTIONAL)	Limit client body size
		std::string				_error_page;			// (MANDATORY)	Setup default error pages. 
		std::vector<Location>	_locations;				// (OPTIONAL)	For setting multiple single-level-deep locations for this Server instance (if present in config file)

	};

	//////////////////////// EXTERNAL HELPER FUNCTIONS BEGIN ////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	void	exit_with_error(const char *color, const std::string& msg);
	

}	// namespace ft

#endif
