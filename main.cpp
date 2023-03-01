# include "includes/Server.hpp"
class Server;

# include "includes/Location.hpp"
class Location;

# include "includes/ConfigParser.hpp"
# include "includes/utils.hpp"

int main (int argc, char *argv[], char *envp[])
{
	if (argc < 2 )
		ft::exit_with_error("Not Enough CMD Parameters!");

	std::vector<ft::Server>	servers;					// we need a Vector of Servers
	ft::ConfigParser		parse(servers, argv[1]);	// Parse input config file to servers
	// RUNNER() GOES HERE

	(void)envp;

	// //FOR TEST PURPOSE
	// 	std::cout << "loc /path 0: " << servers[0].refLocations()[0].readPath() << std::endl;
	// 	std::cout << "loc /path 1: " << servers[0].refLocations()[1].readPath() << std::endl;
	// 	std::cout << "loc /path 2: " << servers[0].refLocations()[2].readPath() << std::endl;
	// 	std::cout << "loc /path 3: " << servers[0].refLocations()[3].readPath() << std::endl;
	// 	std::cout << "loc /path 4: " << servers[0].refLocations()[4].readPath() << std::endl;

	// 	std::cout << "loc methods 0: ";
	// 	for (long unsigned int i = 0; ; ++i) {
	// 		if (servers[0].refLocations()[0].refMethods().size() > i)
	// 			std::cout << servers[0].refLocations()[0].refMethods()[i] << " ";
	// 		else {
	// 			std::cout << std::endl;
	// 			break ;
	// 		}		
	// 	}
	// 	std::cout << "loc methods 1: ";
	// 	for (long unsigned int i = 0; ; ++i) {
	// 		if (servers[0].refLocations()[1].refMethods().size() > i)
	// 			std::cout << servers[0].refLocations()[1].refMethods()[i] << " ";
	// 		else {
	// 			std::cout << std::endl;
	// 			break ;
	// 		}		
	// 	}
	// 	std::cout << "loc methods 2: ";
	// 	for (long unsigned int i = 0; ; ++i) {
	// 		if (servers[0].refLocations()[2].refMethods().size() > i)
	// 			std::cout << servers[0].refLocations()[2].refMethods()[i] << " ";
	// 		else {
	// 			std::cout << std::endl;
	// 			break ;
	// 		}		
	// 	}
	// 	std::cout << "loc methods 3: ";
	// 	for (long unsigned int i = 0; ; ++i) {
	// 		if (servers[0].refLocations()[3].refMethods().size() > i)
	// 			std::cout << servers[0].refLocations()[3].refMethods()[i] << " ";
	// 		else {
	// 			std::cout << std::endl;
	// 			break ;
	// 		}		
	// 	}
	// 	std::cout << "loc methods 4: ";
	// 	for (long unsigned int i = 0; ; ++i) {
	// 		if (servers[0].refLocations()[4].refMethods().size() > i)
	// 			std::cout << servers[0].refLocations()[4].refMethods()[i] << " ";
	// 		else {
	// 			std::cout << std::endl;
	// 			break ;
	// 		}		
	// 	}

	// 	std::cout << "loc root 0: " << servers[0].refLocations()[0].readRoot() << std::endl;
	// 	std::cout << "loc root 1: " << servers[0].refLocations()[1].readRoot() << std::endl;
	// 	std::cout << "loc root 2: " << servers[0].refLocations()[2].readRoot() << std::endl;
	// 	std::cout << "loc root 3: " << servers[0].refLocations()[3].readRoot() << std::endl;
	// 	std::cout << "loc root 4: " << servers[0].refLocations()[4].readRoot() << std::endl;

	// 	std::cout << "loc index 0: " << servers[0].refLocations()[0].readIndex() << std::endl;
	// 	std::cout << "loc index 1: " << servers[0].refLocations()[1].readIndex() << std::endl;
	// 	std::cout << "loc index 2: " << servers[0].refLocations()[2].readIndex() << std::endl;
	// 	std::cout << "loc index 3: " << servers[0].refLocations()[3].readIndex() << std::endl;
	// 	std::cout << "loc index 4: " << servers[0].refLocations()[4].readIndex() << std::endl;

	// 	std::cout << "loc autoindex 0: " << servers[0].refLocations()[0].readAutoind() << std::endl;
	// 	std::cout << "loc autoindex 1: " << servers[0].refLocations()[1].readAutoind() << std::endl;
	// 	std::cout << "loc autoindex 2: " << servers[0].refLocations()[2].readAutoind() << std::endl;
	// 	std::cout << "loc autoindex 3: " << servers[0].refLocations()[3].readAutoind() << std::endl;
	// 	std::cout << "loc autoindex 4: " << servers[0].refLocations()[4].readAutoind() << std::endl;

	// 	std::cout << "loc exec 0: " << servers[0].refLocations()[0].readExec() << std::endl;
	// 	std::cout << "loc exec 1: " << servers[0].refLocations()[1].readExec() << std::endl;
	// 	std::cout << "loc exec 2: " << servers[0].refLocations()[2].readExec() << std::endl;
	// 	std::cout << "loc exec 3: " << servers[0].refLocations()[3].readExec() << std::endl;
	// 	std::cout << "loc exec 4: " << servers[0].refLocations()[4].readExec() << std::endl;

	return 0;
}
