#pragma once
#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <iostream>
# include <string>
# include <vector>
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

		void				writeListen(const std::string& listen);
		const std::string&	readListen( void );

		void				writeName(const std::string& name);
		const std::string&	readName( void );

		void				writeRoot(const std::string& root);
		const std::string&	readRoot( void );

		void				writeMaxBody(const std::string& max_body);
		const std::string&	readMaxBody( void );

		void				writeErrorPage(const std::string& error_page);
		const std::string&	readErrorPage( void );

		std::vector<Location>&	refLocations( void );

	private:		
		std::string				_listen;				// (MANDATORY) Choose the port and host of each ’server’
		std::string				_name;					// (OPTIONAL)	Set the server_names or not
		std::string				_root;					// (MANDATORY)	Define a directory or a file from where the returned file should be searched
		std::string				_max_body;				// (OPTIONAL)	Limit client body size
		std::string				_error_page;			// (MANDATORY)	Setup default error pages. 
		std::vector<Location>	_locations;				// (OPTIONAL)	For setting multiple single-level-deep locations for this Server instance (if present in config file)
	};

}	// namespace ft

#endif
