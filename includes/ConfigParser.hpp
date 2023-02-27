#pragma once
#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

#include "WebServer.hpp"
#include <iostream>
#include "ServerConfig.hpp"
#include <cstring>
class ServerConfig;

//a class that parse the config file and make servers with the results
class ConfigParser {
public:
	ConfigParser( void );
	~ConfigParser( void );
	ConfigParser( const ConfigParser& other );

	ConfigParser& operator= ( const ConfigParser& other );
	void createCluster();
	void createCluster(std::string configFile);
	void createServer(std::string &config, ServerConfig &server);
	std::vector<ServerConfig> getServers();

private:
	std::vector<ServerConfig> servers;
	unsigned int	_nb_server;

};

#endif
