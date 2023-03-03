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

		for ( std::size_t j = 0 && servers.size() > 0; j < servers.size(); ++j ) {
			std::cout << "\n********************SERVER " << j + 1 << " **********************" << std::endl;
			std::cout << "serv IP: " << servers[j].readIp() << std::endl;
			std::cout << "serv Port: " << servers[j].readPort() << std::endl;
			std::cout << "serv SockAddrLen: " << servers[j].readSockAddrLen() << std::endl;
			std::cout << "Network-Byte-Decode serv IP from sockaddr: " << inet_ntoa(servers[j].refSockaddrs().sin_addr) << std::endl;
			std::cout << "Network-Byte-Decode serv Port from sockAddr: " << ntohs(servers[j].refSockaddrs().sin_port) << std::endl;
			std::cout << "serv server_name: " << servers[j].readName() << std::endl;
			std::cout << "serv root: " << servers[j].readRoot() << std::endl;
			std::cout << "serv max_body: " << servers[j].readMaxBody() << std::endl;
			std::cout << "serv error_page: " << servers[j].readErrorPage() << std::endl;

			std::cout << "\nLOCATION" << std::endl;
			std::cout << "loc /path: " << servers[j].refLocations()[0].readPath() << std::endl;
			std::cout << "loc methods: ";
			for (long unsigned int i = 0; ; ++i) {
				if (servers[j].refLocations()[0].refMethods().size() > i)
					std::cout << servers[j].refLocations()[0].refMethods()[i] << " ";
				else {
					std::cout << std::endl;
					break ;
				}		
			}
			std::cout << "loc root: " << servers[j].refLocations()[0].readRoot() << std::endl;
			std::cout << "loc index: " << servers[j].refLocations()[0].readIndex() << std::endl;
			std::cout << "loc autoindex: " << servers[j].refLocations()[0].readAutoind() << std::endl;
			std::cout << "loc exec: " << servers[j].refLocations()[0].readExec() << std::endl;

			std::cout << "\nLOCATION" << std::endl;
			std::cout << "loc /path: " << servers[j].refLocations()[1].readPath() << std::endl;
			std::cout << "loc methods: ";
			for (long unsigned int i = 0; ; ++i) {
				if (servers[j].refLocations()[1].refMethods().size() > i)
					std::cout << servers[j].refLocations()[1].refMethods()[i] << " ";
				else {
					std::cout << std::endl;
					break ;
				}		
			}
			std::cout << "loc root: " << servers[j].refLocations()[1].readRoot() << std::endl;
			std::cout << "loc index: " << servers[j].refLocations()[1].readIndex() << std::endl;
			std::cout << "loc autoindex: " << servers[j].refLocations()[1].readAutoind() << std::endl;
			std::cout << "loc exec: " << servers[j].refLocations()[1].readExec() << std::endl;

			std::cout << "\nLOCATION" << std::endl;
			std::cout << "loc /path: " << servers[j].refLocations()[2].readPath() << std::endl;
			std::cout << "loc methods: ";
			for (long unsigned int i = 0; ; ++i) {
				if (servers[j].refLocations()[2].refMethods().size() > i)
					std::cout << servers[j].refLocations()[2].refMethods()[i] << " ";
				else {
					std::cout << std::endl;
					break ;
				}		
			}
			std::cout << "loc root: " << servers[j].refLocations()[2].readRoot() << std::endl;
			std::cout << "loc index: " << servers[j].refLocations()[2].readIndex() << std::endl;
			std::cout << "loc autoindex: " << servers[j].refLocations()[2].readAutoind() << std::endl;
			std::cout << "loc exec: " << servers[j].refLocations()[2].readExec() << std::endl;

			std::cout << "\nLOCATION" << std::endl;
			std::cout << "loc /path: " << servers[j].refLocations()[3].readPath() << std::endl;
			std::cout << "loc methods: ";
			for (long unsigned int i = 0; ; ++i) {
				if (servers[j].refLocations()[3].refMethods().size() > i)
					std::cout << servers[j].refLocations()[3].refMethods()[i] << " ";
				else {
					std::cout << std::endl;
					break ;
				}		
			}
			std::cout << "loc root: " << servers[j].refLocations()[3].readRoot() << std::endl;
			std::cout << "loc index: " << servers[j].refLocations()[3].readIndex() << std::endl;
			std::cout << "loc autoindex: " << servers[j].refLocations()[3].readAutoind() << std::endl;
			std::cout << "loc exec: " << servers[j].refLocations()[3].readExec() << std::endl;

			std::cout << "\nLOCATION" << std::endl;
			std::cout << "loc /path: " << servers[j].refLocations()[4].readPath() << std::endl;
			std::cout << "loc methods: ";
			for (long unsigned int i = 0; ; ++i) {
				if (servers[j].refLocations()[4].refMethods().size() > i)
					std::cout << servers[j].refLocations()[4].refMethods()[i] << " ";
				else {
					std::cout << std::endl;
					break ;
				}		
			}		
			std::cout << "loc root: " << servers[j].refLocations()[4].readRoot() << std::endl;
			std::cout << "loc index: " << servers[j].refLocations()[4].readIndex() << std::endl;
			std::cout << "loc autoindex: " << servers[j].refLocations()[4].readAutoind() << std::endl;
			std::cout << "loc exec: " << servers[j].refLocations()[4].readExec() << std::endl;
		}

		ft::print_status(ft_GREEN, "Available Test Results Printed Successfully!");

	return 0;
}
