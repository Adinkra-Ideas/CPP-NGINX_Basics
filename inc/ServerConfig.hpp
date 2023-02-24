#pragma once
#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>
# include <utility>
# include <string>
# include <cstdlib>
# include <sys/socket.h>
# include <arpa/inet.h>

class ServerConfig {
public:
	ServerConfig( void );
	~ServerConfig( void );
	ServerConfig( const ServerConfig& other );

	ServerConfig& operator= ( const ServerConfig& other );

	void	setupServer( std::pair<std::string, int>& sock_addr );

	int&				getListen_fd( void );
	sockaddr_in&		getServer_address( void );
	unsigned int&		getServer_address_len( void );

private:
	int						_listen_fd;

	struct sockaddr_in		_server_address;
	unsigned int			_server_address_len;

};

#endif
