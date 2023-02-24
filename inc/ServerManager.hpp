#pragma once
#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

// # include <vector>
# include <iostream>
# include <utility>
# include <string>
# include <sstream>
# include <unistd.h>

# include "ServerConfig.hpp"
class ServerConfig;

# define MAX_QUEUE 20

class ServerManager {
public:
	ServerManager( void );
	~ServerManager( void );
	ServerManager( const ServerManager& other );

	ServerManager& operator= ( const ServerManager& other );

	void    setupServers( std::pair<std::string, int>& sock_addr );
	void    runServers( void );

private:
	ServerConfig	_servers;

};

#endif
