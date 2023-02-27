# include "includes/Server.hpp"
# include "includes/Location.hpp"
# include "includes/ConfigParser.hpp"

int main (int argc, char *argv[], char *envp[])
{
	if (argc < 2 )
		ft::exit_with_error(ft_RED, "Not Enough CMD Parameters!");

	std::vector<ft::Server>	servers;					// we need a Vector of Servers
	ft::ConfigParser		parser(servers, argv[1]);	// Parse input config file and init server(s)

	

	(void)envp;


	return 0;
}
