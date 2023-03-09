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

	typedef std::vector<ft::Server> vector_of_servers;
	typedef ft::ConfigParser		parser_object;
	if (argc != 2)
		ft::exit_with_error("Not Enough CMD Parameters!");

	vector_of_servers	servers;
	parser_object		parser(servers, argv[1]);
	ServerManager					master;
	master.setupServers(servers);
	master.runServers();
	

	return (0);
}

