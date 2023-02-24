# include <iostream>

# include "ConfigParser.hpp"
# include "ServerManager.hpp"
# include "ServerConfig.hpp"

int main( void ) {
	std::pair<std::string, int>		sock_addr("127.0.0.1", 8080);
	ConfigParser					cluster;
	ServerManager					master;

	master.setupServers(sock_addr);
	master.runServers();
	

	return (0);
}

