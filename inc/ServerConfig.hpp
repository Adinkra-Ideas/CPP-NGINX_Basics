#pragma once
#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>
# include <utility>
# include <string>
# include <cstdlib>
# include <sys/socket.h>
# include <arpa/inet.h>

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
// private:
	int						_listen_fd;
	in_addr_t				_host;
	uint16_t				_port;
	struct sockaddr_in		_server_address;
	unsigned int			_server_address_len;

};

#endif
