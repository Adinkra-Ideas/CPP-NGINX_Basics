# include "ServerManager.hpp"

ServerManager::ServerManager( void ) {}

ServerManager::~ServerManager( void ) {}

ServerManager::ServerManager( const ServerManager& other ) {
	_servers = other._servers;
}

ServerManager& ServerManager::operator= ( const ServerManager& other ) {
	_servers = other._servers;
	return *this;
}

//TODO getting also all the FD and bindings
void    ServerManager::setupServers(std::vector<ServerConfig> servers)
{
	this->_servers = servers;

	for (std::vector<ServerConfig>::iterator iter = this->_servers.begin(); iter != this->_servers.end(); iter++)
	{

		iter->setupServer();
	}
	
}

//TODO coding the running of the server
void    ServerManager::runServers( void ) {
	int outbound_socket;
	long bytesSent;
	std::string	webpage("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"https://fonts.googleapis.com/css2?family=Kaushan+Script&family=Montserrat:wght@400;700&display=swap\" rel=\"stylesheet\"><link href=\"https://drive.google.com/uc?export=view&id=1Xugr6sMP2KHXBkaLNlZFxRnBLNDQkB5R\" rel=\"stylesheet\"><title>Fusion</title></head><body><header class=\"header\"><div class=\"container\"><div class=\"header__inner\"><a class=\"nav__link\" href=\"#\">Tour</a><nav class=\"nav\"><a class=\"nav__link\" href=\"#\">Home</a><a class=\"nav__link\" href=\"#\">Services</a><a class=\"nav__link\" href=\"#\">About</a><a class=\"nav__link\" href=\"#\">Contact</a><a class=\"nav__link\" href=\"#\">Cookies Test</a><a class=\"nav__link\" href=\"#\">Account</a></nav></div></div></header><div class=\"intro\"><div class=\"container\"><div class=\"intro__inner\"><h2 class=\"inner__uptitle\">Fusion travel</h2><h1 class=\"intro__title\">Let's Enjoy Your Trip In UAE</h1></div></div><footer><p><span class=\"highlight\">&#169; 2022 by AMANIX</p></footer></div></body></html>");
	std::ostringstream tmp;
	std::string httpResponse;

	tmp << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "
		<< webpage.size() << "\n\n" << webpage;
	httpResponse = tmp.str();
	std::cout << _servers[0].getListen_fd() << std::endl;
	if ( listen(_servers[0].getListen_fd(), MAX_QUEUE) )
	{
        // Logger::logMsg(RED, CONSOLE_OUTPUT, "webserv: listen error: %s   Closing....", strerror(errno));
        std::cerr << "\nwebserv: listen error:  Closing...." << std::endl;
		exit(1);
    }

	std::cout << "\n====== webserv: Listening on Socket Address ====== " <<
				inet_ntoa(_servers[0].getServer_address().sin_addr) << ":" <<
				ntohs(_servers[0].getServer_address().sin_port) << std::endl;

	while (true)
	{
		{
			std::cout << "\n====== Waiting for a new connection request ======\n\n\n";

			outbound_socket = accept(_servers[0].getListen_fd(),
					(sockaddr *)&_servers[0].getServer_address(),
					&_servers[0].getServer_address_len());
				
			std::cout << "------ New Outbound Socket Address " << outbound_socket
				<< " Opened for Client "
				<< inet_ntoa(_servers[0].getServer_address().sin_addr) << ":" 
				<< ntohs(_servers[0].getServer_address().sin_port) << " ------\n\n";
			
			bytesSent = write(outbound_socket, httpResponse.data(), httpResponse.size());
			if ( bytesSent >= 0 &&
					static_cast<long unsigned int>(bytesSent) == httpResponse.size() )
				std::cout << "------ Server Response sent to client ------\n\n";
			else
				std::cout << "Error sending response to client";
		} // Nested Scope performing the role of C close() for outbound_socket FD
	}
	
}
