# include <iostream>

# include "ConfigParser.hpp"
# include "ServerManager.hpp"
# include "ServerConfig.hpp"

int main( int argc, char **argv )
{

	ConfigParser					cluster;
	ServerManager					master;
	if (argc == 1)
		cluster.createCluster();
	else if (argc == 2)
		std::cout << argv[1] << std::endl;
	else
		exit(1);

	master.setupServers(cluster.getServers());
	master.runServers();
	

	return (0);
}

