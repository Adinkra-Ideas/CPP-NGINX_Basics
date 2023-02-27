#pragma once
#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP



#include <WebServer.hpp>
# include "ServerConfig.hpp"
class ServerConfig;

# define MAX_QUEUE 20

//the webserver
//starts and runs the servers
class ServerManager {
public:
	ServerManager( void );
	~ServerManager( void );
	ServerManager( const ServerManager& other );

	ServerManager& operator= ( const ServerManager& other );

	void    setupServers(std::vector<ServerConfig> servers);
	void    runServers( void );

private:
	std::vector<ServerConfig>	_servers;

};

#endif
