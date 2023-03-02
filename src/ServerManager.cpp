# include "ServerManager.hpp"

ServerManager::ServerManager( void ) : biggest_fd(0){
	FD_ZERO(&recive_fds);
    FD_ZERO(&write_fds);
}

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
		listen(iter->getListen_fd(), 512);
		fcntl(iter->getListen_fd(), F_SETFL, O_NONBLOCK);
		FD_SET(iter->getListen_fd(), &this->recive_fds);
		if (iter->getListen_fd() > this->biggest_fd)
			this->biggest_fd = iter->getListen_fd();
		this->running_servers[iter->getListen_fd()] = *iter;
	}
	
}

//TODO coding the running of the server
void    ServerManager::runServers( void ) {
	fd_set recive_fds_tmp;
	fd_set write_fds_tmp;
	int ret;
	struct timeval timeout_timer;

	while (true)
	{
		timeout_timer.tv_sec = 1;
		timeout_timer.tv_usec = 0;
		recive_fds_tmp = this->recive_fds;
		write_fds_tmp = this->write_fds;
		ret = select(this->biggest_fd + 1, &recive_fds_tmp, &write_fds_tmp, NULL, &timeout_timer);
		if (ret < 0)
		{
			std::cout << "error in select" << std::endl;
			exit (1);
		}
		for (int i = 0; i <= this->biggest_fd; i++)
		{
			if(FD_ISSET(i, &recive_fds_tmp) && this->running_servers.count(i))
				acceptConnection(this->running_servers.find(i)->second);
			else if (FD_ISSET(i, &recive_fds_tmp) && this->connected_clients.count(i))
				readRequest(i, this->connected_clients[i]);
			else if (FD_ISSET(i, &write_fds_tmp) && this->connected_clients.count(i))
			{
				std::cout << " send response " << std::endl;
					FD_CLR(i, &this->write_fds);
					if(i == this->biggest_fd)
						this->biggest_fd--;
					this->connected_clients.erase(i);
			}
				

		}
		

	}
	// int outbound_socket;
	// long bytesSent;
	// std::string	webpage("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"https://fonts.googleapis.com/css2?family=Kaushan+Script&family=Montserrat:wght@400;700&display=swap\" rel=\"stylesheet\"><link href=\"https://drive.google.com/uc?export=view&id=1Xugr6sMP2KHXBkaLNlZFxRnBLNDQkB5R\" rel=\"stylesheet\"><title>Fusion</title></head><body><header class=\"header\"><div class=\"container\"><div class=\"header__inner\"><a class=\"nav__link\" href=\"#\">Tour</a><nav class=\"nav\"><a class=\"nav__link\" href=\"#\">Home</a><a class=\"nav__link\" href=\"#\">Services</a><a class=\"nav__link\" href=\"#\">About</a><a class=\"nav__link\" href=\"#\">Contact</a><a class=\"nav__link\" href=\"#\">Cookies Test</a><a class=\"nav__link\" href=\"#\">Account</a></nav></div></div></header><div class=\"intro\"><div class=\"container\"><div class=\"intro__inner\"><h2 class=\"inner__uptitle\">Fusion travel</h2><h1 class=\"intro__title\">Let's Enjoy Your Trip In UAE</h1></div></div><footer><p><span class=\"highlight\">&#169; 2022 by AMANIX</p></footer></div></body></html>");
	// std::ostringstream tmp;
	// std::string httpResponse;

	// tmp << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "
	// 	<< webpage.size() << "\n\n" << webpage;
	// httpResponse = tmp.str();
	// std::cout << _servers[0].getListen_fd() << std::endl;
	// if ( listen(_servers[0].getListen_fd(), MAX_QUEUE) )
	// {
    //     // Logger::logMsg(RED, CONSOLE_OUTPUT, "webserv: listen error: %s   Closing....", strerror(errno));
    //     std::cerr << "\nwebserv: listen error:  Closing...." << std::endl;
	// 	exit(1);
    // }

	// std::cout << "\n====== webserv: Listening on Socket Address ====== " <<
	// 			inet_ntoa(_servers[0].getServer_address().sin_addr) << ":" <<
	// 			ntohs(_servers[0].getServer_address().sin_port) << std::endl;

	// while (true)
	// {
	// 	{
	// 		std::cout << "\n====== Waiting for a new connection request ======\n\n\n";

	// 		outbound_socket = accept(_servers[0].getListen_fd(),
	// 				(sockaddr *)&_servers[0].getServer_address(),
	// 				&_servers[0].getServer_address_len());
				
	// 		std::cout << "------ New Outbound Socket Address " << outbound_socket
	// 			<< " Opened for Client "
	// 			<< inet_ntoa(_servers[0].getServer_address().sin_addr) << ":" 
	// 			<< ntohs(_servers[0].getServer_address().sin_port) << " ------\n\n";
			
	// 		bytesSent = write(outbound_socket, httpResponse.data(), httpResponse.size());
	// 		if ( bytesSent >= 0 &&
	// 				static_cast<long unsigned int>(bytesSent) == httpResponse.size() )
	// 			std::cout << "------ Server Response sent to client ------\n\n";
	// 		else
	// 			std::cout << "Error sending response to client";
	// 	} // Nested Scope performing the role of C close() for outbound_socket FD
	// }
	
}

void ServerManager::acceptConnection(ServerConfig &server)
{
	std::cout << "------ New Outbound Connection "
				<< inet_ntoa(server.getServer_address().sin_addr) << ":" 
				<< ntohs(server.getServer_address().sin_port) << " ------\n\n";
	    struct sockaddr_in client_address;
    long  client_address_size = sizeof(client_address);
    int client_sock;
    Client  new_client(server);
	client_sock = accept(server.getListen_fd(), (struct sockaddr *)&client_address,(socklen_t*)&client_address_size);
	FD_SET(client_sock, &this->recive_fds);
	if (client_sock > this->biggest_fd)
		this->biggest_fd = client_sock;
	new_client.setSocket(client_sock);
	this->connected_clients[client_sock] = new_client;
}

void ServerManager::readRequest(int fd, Client &client)
{
	char buffer[BUFFER_SIZE];
	std::cout << "------ New ReadRequest from: " << fd << std::endl;
	int bytes_read;
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	std::string request(buffer, bytes_read);
	Request *req = client.getRequest();
	req->parse(request);
	FD_CLR(fd, &this->recive_fds);
	if(fd == this->biggest_fd)
		this->biggest_fd--;
	FD_SET(fd, &this->write_fds);
	if (fd > this->biggest_fd)
		this->biggest_fd = fd;

}
