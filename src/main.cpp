# include <iostream>

# include "ConfigParser.hpp"
# include "ServerManager.hpp"
# include "Server.hpp"

int main( int argc, char **argv )
{
	http::ServerManager		master;

	if ( argc != 2 )
		http::exit_with_error("Wrong CMD Parameter Count!");

	master.parseConfig(argv[1]);	
	master.setupServers();
	master.runServers();

	return (0);
}
