# include "includes/Server.hpp"
# include "includes/Location.hpp"
# include "includes/ConfigParser.hpp"
# include "includes/utils.hpp"

int main (int argc, char *argv[], char *envp[])
{
	if (argc < 2 )
		ft::exit_with_error("Not Enough CMD Parameters!");

	std::vector<ft::Server>	servers;					// we need a Vector of Servers
	ft::ConfigParser		parse(servers, argv[1]);	// Parse input config file and init server(s)							


	(void)envp;


	return 0;
}
