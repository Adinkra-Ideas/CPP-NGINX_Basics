# include "includes/Server.hpp"
# include "includes/Location.hpp"

int main (int argc, char *argv[], char *envp[])
{
	std::vector<ft::Server> test;	// we need a Vector of Servers

	if (argc < 2 || ft::parse_config(argv[1]) == 1 )
		ft::exit_with_error(ft_RED, "Invalid CMD Params or Config File Unreadable!");
	
	(void)argv;
	(void)envp;


	return 0;
}
