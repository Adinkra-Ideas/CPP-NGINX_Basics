#pragma once
#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>
# include <utility>
# include <string>
# include <cstdlib>
# include <sys/socket.h>
# include <arpa/inet.h>
# include "Location.hpp"

//a class that holds the information for a server
//also can start the server
class ServerConfig {
public:
	ServerConfig( void );
	~ServerConfig( void );
	ServerConfig( const ServerConfig& other );

	ServerConfig& operator= ( const ServerConfig& other );

	void	setupServer();

	int&				getListen_fd( void );
	sockaddr_in&		getServer_address( void );
	unsigned int&		getServer_address_len( void );
	void setHost(std::string host);
	void setPort(std::string port);
 private:
	int						_listen_fd;
	in_addr_t				_host;
	uint16_t				_port;
	struct sockaddr_in		_server_address;
	unsigned int			_server_address_len;
	//TODO from server the varibales
	// struct	sockaddr_in		_socketAddress;			// (MANDATORY) Choose the port and host of each ’server’
	// std::string				_server_name;			// (OPTIONAL)	Set the server_names or not
	// std::string				_root;					// (MANDATORY)	Define a directory or a file from where the returned file should be searched
	// std::size_t				_client_max_body_size;	// (OPTIONAL)	Limit client body size
	// std::string				_error_page;			// (MANDATORY)	Setup default error pages. 
	// std::vector<Location>	_locations;				// (OPTIONAL)	For setting multiple single-level-deep locations for this Server instance (if present in config file)

};

#endif
