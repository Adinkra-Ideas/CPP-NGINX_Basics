# include "ServerManager.hpp"

namespace http {

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
	void    ServerManager::setupServers(std::vector<http::Server> servers)
	{
		this->_servers = servers;

		for (std::vector<http::Server>::iterator iter = this->_servers.begin(); iter != this->_servers.end(); iter++)
		{
			iter->bindServerSockAddr();
			iter->startListen(MAX_QUEUE);

			fcntl(iter->readInSock(), F_SETFL, O_NONBLOCK); // what does this do?
			FD_SET(iter->readInSock(), &this->recive_fds);
			if (iter->readInSock() > this->biggest_fd)
				this->biggest_fd = iter->readInSock();
			this->running_servers[iter->readInSock()] = *iter;
		}
		
	}

	//TODO timeout checker for clients
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
				exit(1);
			}
			for (int i = 0; i <= this->biggest_fd; i++)
			{
				if(FD_ISSET(i, &recive_fds_tmp) && this->running_servers.count(i))
					acceptConnection(this->running_servers.find(i)->second);
				else if (FD_ISSET(i, &recive_fds_tmp) && this->connected_clients.count(i))
					readRequest(i, this->connected_clients[i]);
				else if (FD_ISSET(i, &write_fds_tmp) && this->connected_clients.count(i))
				{
					sendResponce(i, this->connected_clients[i]);
				}
			}
		}	
	}


	void ServerManager::acceptConnection(http::Server &server)
	{
		std::cout << "------ New Outbound Connection "
					<< inet_ntoa(server.refSockaddrs().sin_addr) << ":" 
					<< ntohs(server.refSockaddrs().sin_port) << " ------\n\n";
			struct sockaddr_in client_address;
		long  client_address_size = sizeof(client_address);
		int client_sock;
		Client  new_client(server);
		client_sock = accept(server.readInSock(), (struct sockaddr *)&client_address,(socklen_t*)&client_address_size);
		FD_SET(client_sock, &this->recive_fds);
		if (client_sock > this->biggest_fd)
			this->biggest_fd = client_sock;
		new_client.setSocket(client_sock);
		this->connected_clients[client_sock] = new_client;
	}

	void ServerManager::readRequest(int fd, Client &client)
	{
		char buffer[BUFFER_SIZE];
		int bytes_read;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		std::string request(buffer, bytes_read);
		client.updateTime();
		client.request.parse(request);
		if (client.request.parsingFinished())
		{
			assign_server_for_response(client);
			client.buildResponse();
			FD_CLR(fd, &this->recive_fds);
			if(fd == this->biggest_fd)
				this->biggest_fd--;
			FD_SET(fd, &this->write_fds);
			if (fd > this->biggest_fd)
				this->biggest_fd = fd;
		}

	}

	void ServerManager::sendResponce(int fd, Client &client)
	{

		long bytesSent;
		bytesSent = send(client.getSocket(), client.response.response_content.data(), client.response.response_content.size(), 0);
		if ( bytesSent >= 0 &&
				static_cast<long unsigned int>(bytesSent) == client.response.response_content.size() )
				std::cout << "------ Server Response sent to client ------\n\n";
		else
			std::cout << "Error sending response to client";
		FD_CLR(fd, &this->write_fds);
		if(fd == this->biggest_fd)
			this->biggest_fd--;
		this->connected_clients.erase(fd);
	}

	void    ServerManager::assign_server_for_response(Client &client)
	{
		for(std::vector<http::Server>::iterator it = this->_servers.begin(); it != this->_servers.end(); ++it)
		{
			if (client.getServer().readIp() == it->readIp() &&
				client.getServer().readPort() == it->readPort() &&
				client.request.getServerName() == it->readName()
				)
				{
					client.setServer(*it);
					break ;
				}
		}
	}

} // namespace ft
