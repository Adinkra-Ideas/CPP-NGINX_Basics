# include <iostream>

# include "ConfigParser.hpp"
# include "ServerManager.hpp"
# include "Server.hpp"

//TODO error handling
//TODO changing namespace?
//TODO adding cgi
//TODO adding timeout on connections
//TODO adding a logger class?
//TODO adding signal handlers?
//TODO checking leaks
//TODO check when to close fd of client
int main( int argc, char **argv )
{
	http::ServerManager				master;

	if (argc != 2)
		http::exit_with_error("Not Enough CMD Parameters!");

	master.parseConfig(argv[1]);	
	master.setupServers();
	master.runServers();

	return (0);
}

