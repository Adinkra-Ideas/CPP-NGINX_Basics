# include "../includes/Server.hpp"
# include "../includes/Location.hpp"
# include "../includes/ConfigParser.hpp"
# include "../includes/utils.hpp"

int main (int argc, char *argv[], char *envp[])
{
	if (argc < 2 )
		ft::exit_with_error("Not Enough CMD Parameters!");

	std::vector<ft::Server>	servers;					// we need a Vector of Servers
	ft::ConfigParser		parse(servers, argv[1]);	// Parse input config file to servers

	(void)envp;

		ft::print_status(ft_YELLOW, "Printing Test Results...");

		std::cout << "\n********************FIRST SERVER**********************" << std::endl;
		std::cout << "serv listen: " << servers[0].readListen() << std::endl;
		std::cout << "serv server_name: " << servers[0].readName() << std::endl;
		std::cout << "serv root: " << servers[0].readRoot() << std::endl;
		std::cout << "serv max_body: " << servers[0].readMaxBody() << std::endl;
		std::cout << "serv error_page: " << servers[0].readErrorPage() << std::endl;

		std::cout << "\nLOCATION" << std::endl;
		std::cout << "loc /path: " << servers[0].refLocations()[0].readPath() << std::endl;
		std::cout << "loc methods: ";
		for (long unsigned int i = 0; ; ++i) {
			if (servers[0].refLocations()[0].refMethods().size() > i)
				std::cout << servers[0].refLocations()[0].refMethods()[i] << " ";
			else {
				std::cout << std::endl;
				break ;
			}		
		}
		std::cout << "loc root: " << servers[0].refLocations()[0].readRoot() << std::endl;
		std::cout << "loc index: " << servers[0].refLocations()[0].readIndex() << std::endl;
		std::cout << "loc autoindex: " << servers[0].refLocations()[0].readAutoind() << std::endl;
		std::cout << "loc exec: " << servers[0].refLocations()[0].readExec() << std::endl;

		std::cout << "\nLOCATION" << std::endl;
		std::cout << "loc /path: " << servers[0].refLocations()[1].readPath() << std::endl;
		std::cout << "loc methods: ";
		for (long unsigned int i = 0; ; ++i) {
			if (servers[0].refLocations()[1].refMethods().size() > i)
				std::cout << servers[0].refLocations()[1].refMethods()[i] << " ";
			else {
				std::cout << std::endl;
				break ;
			}		
		}
		std::cout << "loc root: " << servers[0].refLocations()[1].readRoot() << std::endl;
		std::cout << "loc index: " << servers[0].refLocations()[1].readIndex() << std::endl;
		std::cout << "loc autoindex: " << servers[0].refLocations()[1].readAutoind() << std::endl;
		std::cout << "loc exec: " << servers[0].refLocations()[1].readExec() << std::endl;

		std::cout << "\nLOCATION" << std::endl;
		std::cout << "loc /path: " << servers[0].refLocations()[2].readPath() << std::endl;
		std::cout << "loc methods: ";
		for (long unsigned int i = 0; ; ++i) {
			if (servers[0].refLocations()[2].refMethods().size() > i)
				std::cout << servers[0].refLocations()[2].refMethods()[i] << " ";
			else {
				std::cout << std::endl;
				break ;
			}		
		}
		std::cout << "loc root: " << servers[0].refLocations()[2].readRoot() << std::endl;
		std::cout << "loc index: " << servers[0].refLocations()[2].readIndex() << std::endl;
		std::cout << "loc autoindex: " << servers[0].refLocations()[2].readAutoind() << std::endl;
		std::cout << "loc exec: " << servers[0].refLocations()[2].readExec() << std::endl;

		std::cout << "\nLOCATION" << std::endl;
		std::cout << "loc /path: " << servers[0].refLocations()[3].readPath() << std::endl;
		std::cout << "loc methods: ";
		for (long unsigned int i = 0; ; ++i) {
			if (servers[0].refLocations()[3].refMethods().size() > i)
				std::cout << servers[0].refLocations()[3].refMethods()[i] << " ";
			else {
				std::cout << std::endl;
				break ;
			}		
		}
		std::cout << "loc root: " << servers[0].refLocations()[3].readRoot() << std::endl;
		std::cout << "loc index: " << servers[0].refLocations()[3].readIndex() << std::endl;
		std::cout << "loc autoindex: " << servers[0].refLocations()[3].readAutoind() << std::endl;
		std::cout << "loc exec: " << servers[0].refLocations()[3].readExec() << std::endl;

		std::cout << "\nLOCATION" << std::endl;
		std::cout << "loc /path: " << servers[0].refLocations()[4].readPath() << std::endl;
		std::cout << "loc methods: ";
		for (long unsigned int i = 0; ; ++i) {
			if (servers[0].refLocations()[4].refMethods().size() > i)
				std::cout << servers[0].refLocations()[4].refMethods()[i] << " ";
			else {
				std::cout << std::endl;
				break ;
			}		
		}		
		std::cout << "loc root: " << servers[0].refLocations()[4].readRoot() << std::endl;
		std::cout << "loc index: " << servers[0].refLocations()[4].readIndex() << std::endl;
		std::cout << "loc autoindex: " << servers[0].refLocations()[4].readAutoind() << std::endl;
		std::cout << "loc exec: " << servers[0].refLocations()[4].readExec() << std::endl;

		std::cout << "\n\n\n********************SECOND SERVER**********************" << std::endl;
		std::cout << "serv listen: " << servers[1].readListen() << std::endl;
		std::cout << "serv server_name: " << servers[1].readName() << std::endl;
		std::cout << "serv root: " << servers[1].readRoot() << std::endl;
		std::cout << "serv max_body: " << servers[1].readMaxBody() << std::endl;
		std::cout << "serv error_page: " << servers[1].readErrorPage() << std::endl;

		std::cout << "\nLOCATION" << std::endl;
		std::cout << "loc /path: " << servers[1].refLocations()[0].readPath() << std::endl;
		std::cout << "loc methods: ";
		for (long unsigned int i = 0; ; ++i) {
			if (servers[1].refLocations()[0].refMethods().size() > i)
				std::cout << servers[1].refLocations()[0].refMethods()[i] << " ";
			else {
				std::cout << std::endl;
				break ;
			}		
		}
		std::cout << "loc root: " << servers[1].refLocations()[0].readRoot() << std::endl;
		std::cout << "loc index: " << servers[1].refLocations()[0].readIndex() << std::endl;
		std::cout << "loc autoindex: " << servers[1].refLocations()[0].readAutoind() << std::endl;
		std::cout << "loc exec: " << servers[1].refLocations()[0].readExec() << std::endl;

		std::cout << "\nLOCATION" << std::endl;
		std::cout << "loc /path: " << servers[1].refLocations()[1].readPath() << std::endl;
		std::cout << "loc methods: ";
		for (long unsigned int i = 0; ; ++i) {
			if (servers[1].refLocations()[1].refMethods().size() > i)
				std::cout << servers[1].refLocations()[1].refMethods()[i] << " ";
			else {
				std::cout << std::endl;
				break ;
			}		
		}
		std::cout << "loc root: " << servers[1].refLocations()[1].readRoot() << std::endl;
		std::cout << "loc index: " << servers[1].refLocations()[1].readIndex() << std::endl;
		std::cout << "loc autoindex: " << servers[1].refLocations()[1].readAutoind() << std::endl;
		std::cout << "loc exec: " << servers[1].refLocations()[1].readExec() << std::endl;

		std::cout << "\nLOCATION" << std::endl;
		std::cout << "loc /path: " << servers[1].refLocations()[2].readPath() << std::endl;
		std::cout << "loc methods: ";
		for (long unsigned int i = 0; ; ++i) {
			if (servers[1].refLocations()[2].refMethods().size() > i)
				std::cout << servers[1].refLocations()[2].refMethods()[i] << " ";
			else {
				std::cout << std::endl;
				break ;
			}		
		}
		std::cout << "loc root: " << servers[1].refLocations()[2].readRoot() << std::endl;
		std::cout << "loc index: " << servers[1].refLocations()[2].readIndex() << std::endl;
		std::cout << "loc autoindex: " << servers[1].refLocations()[2].readAutoind() << std::endl;
		std::cout << "loc exec: " << servers[1].refLocations()[2].readExec() << std::endl;

		std::cout << "\nLOCATION" << std::endl;
		std::cout << "loc /path: " << servers[1].refLocations()[3].readPath() << std::endl;
		std::cout << "loc methods: ";
		for (long unsigned int i = 0; ; ++i) {
			if (servers[1].refLocations()[3].refMethods().size() > i)
				std::cout << servers[1].refLocations()[3].refMethods()[i] << " ";
			else {
				std::cout << std::endl;
				break ;
			}		
		}
		std::cout << "loc root: " << servers[1].refLocations()[3].readRoot() << std::endl;
		std::cout << "loc index: " << servers[1].refLocations()[3].readIndex() << std::endl;
		std::cout << "loc autoindex: " << servers[1].refLocations()[3].readAutoind() << std::endl;
		std::cout << "loc exec: " << servers[1].refLocations()[3].readExec() << std::endl;

		std::cout << "\nLOCATION" << std::endl;
		std::cout << "loc /path: " << servers[1].refLocations()[4].readPath() << std::endl;
		std::cout << "loc methods: ";
		for (long unsigned int i = 0; ; ++i) {
			if (servers[1].refLocations()[4].refMethods().size() > i)
				std::cout << servers[1].refLocations()[4].refMethods()[i] << " ";
			else {
				std::cout << std::endl;
				break ;
			}		
		}		
		std::cout << "loc root: " << servers[1].refLocations()[4].readRoot() << std::endl;
		std::cout << "loc index: " << servers[1].refLocations()[4].readIndex() << std::endl;
		std::cout << "loc autoindex: " << servers[1].refLocations()[4].readAutoind() << std::endl;
		std::cout << "loc exec: " << servers[1].refLocations()[4].readExec() << std::endl;

		ft::print_status(ft_GREEN, "Test Results Printed Successfully!");

	return 0;
}
