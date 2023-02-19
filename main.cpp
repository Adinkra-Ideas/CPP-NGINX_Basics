# include <iostream>

# include "inc/ConfigParser.hpp"
# include "inc/ServerManager.hpp"
# include "inc/ServerConfig.hpp"

int main( void ) {
	std::pair<std::string, int>		sock_addr("127.0.0.1", 8080);
	ConfigParser					cluster;
	ServerManager					master;

	master.setupServers(sock_addr);
	master.runServers();
	

	return (0);
}
