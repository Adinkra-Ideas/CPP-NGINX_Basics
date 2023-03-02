# include "includes/Server.hpp"
# include "includes/Location.hpp"
# include "includes/ConfigParser.hpp"
# include "includes/utils.hpp"

int main (int argc, char *argv[], char *envp[])
{
	typedef std::vector<ft::Server> vector_of_servers;
	typedef ft::ConfigParser		parser_object;
	
	if (argc < 2 )
		ft::exit_with_error("Not Enough CMD Parameters!");

	vector_of_servers	servers;					// we need a Vector of Servers
	parser_object		parser(servers, argv[1]);		// Parse input config file to servers

	// ft::InitServers(servers);					// This object should initialize servers to start listening for incoming connections


	(void)envp;

	return 0;
}
