# include "ServerManager.hpp"

namespace http {

	ServerManager::ServerManager( void ) : _biggest_fd(0){
		FD_ZERO(&_received_fds);
		FD_ZERO(&_write_fds);
	}

	ServerManager::~ServerManager( void ) {}

	ServerManager::ServerManager( const ServerManager& other ) {
		_servers = other._servers;
	}

	ServerManager& ServerManager::operator= ( const ServerManager& other ) {
		_servers = other._servers;
		return *this;
	}

	// ****************	CONSTRUCTORS && OPERATORS ENDS	**********************
	//////////////////////////////////////////////////////////////////////////
	// ***********	Managing the Servers Methods BEGINS		******************

	// **********************************************
	// Calls the http::ConfigParser object AKA		*
	// parser_function_object to parse the config	*
	// filepath being dereferenced by path pointer	*
	// **********************************************
	void	ServerManager::parseConfig( const char *path ) {
		parser_function_object	parser(_servers, path);
		// std::cout << " port " << _servers[0].readPort() << std::endl;
		// std::cout << " port " << _servers[1].readPort() << std::endl;
	}

	// **********************************************************
	// Binds each Server element in the vector object _server	*
	// To a socket FD. Then sets up listen() foreach, then		*
	// initializes the map object _running_servers				*
	// for each, using FD as Key, and Server (running) as Val	*
	// **********************************************************
	void    ServerManager::setupServers( void )
	{
		std::ostringstream	msg;

		for (std::vector<http::Server>::iterator iter = this->_servers.begin(); iter != this->_servers.end(); ++iter)
		{
			iter->bindServerSockAddr();

			// Checking if the FD of this server's socket Address
			// will be okay to use in fd_set data type
			if ( iter->readInSock() >= FD_SETSIZE ) {
				msg << "Allocated Socket FD " << iter->readInSock() 
					<< " is Greater than FD_SETSIZE value on Host Kernel"; 
				exit_with_error(msg.str());
			}

			iter->startListen(MAX_QUEUE);

			// Setting the Socket FD flag to Non-Block Mode
			if ( fcntl(iter->readInSock(), F_SETFL, O_NONBLOCK) == -1 ) {
				msg << "Setting Socket FD "
					<< iter->readInSock() 
					<< " to Non-Block Mode Failed"; 
				exit_with_error(msg.str());
			}

			// Adding the Socket FD to list of received FD stored in _received_fds
			FD_SET(iter->readInSock(), &this->_received_fds);

			if (iter->readInSock() > this->_biggest_fd)
				this->_biggest_fd = iter->readInSock();
			this->_running_servers[iter->readInSock()] = *iter;
		}
	}

	//TODO timeout checker for clients
	void    ServerManager::runServers( void ) {
		fd_set			_received_fds_tmp;
		fd_set 			_write_fds_tmp;
		int 			ret;
		struct timeval	timeout_timer;

		while (true)
		{
			timeout_timer.tv_sec = 1;
			timeout_timer.tv_usec = 0;
			_received_fds_tmp = this->_received_fds;
			_write_fds_tmp = this->_write_fds;

			// Select() is blocking until timeout_timer
			ret = select(this->_biggest_fd + 1, &_received_fds_tmp, &_write_fds_tmp, NULL, &timeout_timer);
			if (ret < 0) // If select(9 fails. But What if select() times out?
				exit_with_error("error in select");

			// The idea behind this for() loop is to process the FD's that select() dumped
			// into _received_fds_tmp (AKA, these FD's have incoming client requests), then
			// send "dynamic" HTTPresponse to their outbound socket FD addr, then add their
			// outbound socket FD addr to this->_write_fds so that in the next iteration, 
			// select() will also dump their outbound socket FD to _write_fds_tmp.
			for (int i = 0; i <= this->_biggest_fd; i++)
			{
				// If a socket FD received an incoming request, then the Server config object
				// mapped to the socket FD is passed to acceptConnection()
				if( FD_ISSET(i, &_received_fds_tmp) && this->_running_servers.count(i) )
					acceptConnection(this->_running_servers.find(i)->second);

				// Checking if acceptConnection() has already accepted this FD successfully
				// and initialized a connected_clients Map element for it
				// but here, first i is server FD while second i is client FD. How possible?
				// else if ( FD_ISSET(i, &_received_fds_tmp) && this->connected_clients.count(i) )
				else if ( FD_ISSET(i, &_received_fds_tmp) && this->connected_clients.count(i) )
					readRequest(i, this->connected_clients[i]);
				
				// We're here. But first, we need to complete understanding what's going on in readRequest(). the request and response objects
				else if (FD_ISSET(i, &_write_fds_tmp) && this->connected_clients.count(i))
				{
					sendResponce(i, this->connected_clients[i]);
				}
				// else if (FD_ISSET(i, &_except_fds) // This is for managing exception if necessary
			}
			checkTimeout();
		}	
	}
	void ServerManager::checkTimeout()
	{
		for(std::map<int, Client>::iterator iter = this->connected_clients.begin(); iter != this->connected_clients.end(); ++iter)
		{
			if (difftime(time(NULL), iter->second.getupdateTime()) > TIMEOUTTIME)
			{
				//TODO send error msg to client
				print_status(ft_GREEN, "Timeout Closing connection to Client because of timeout");
				closeConnectionToClient(iter->first);
			}
		}
	}
	void ServerManager::closeConnectionToClient(const int fd)
	{
		if (FD_ISSET(fd, &this->_received_fds))
			removeFDToSet(fd, this->_received_fds);
		if (FD_ISSET(fd, &this->_write_fds))
			removeFDToSet(fd, this->_write_fds);
		close(fd);
		this->connected_clients.erase(fd);

	}
	// **********************************************************
	// rcvd_fds_tmp param1 is a reference to the fd_set object 	*
	// that select() generated through its select(param2)		*
	// server param is the Server this client connected to.		*
	// Call accept() using server's incoming Socket FD,	then	*
	// Create a new Client object that holds the copy of 		*
	// server and outgoing Socket Fd that can be used to reach 	*
	// this client.												*
	// A Map element is added to this->connected_clients mapping*
	// this client's outgoing Socket Fd to his Client object	*
	// **********************************************************
	void ServerManager::acceptConnection(http::Server &server)
	{
		std::ostringstream	msg;
	
		msg << "New Client is Connecting to Socket Address "
			<< inet_ntoa(server.refSockaddrs().sin_addr) << ":" 
			<< ntohs(server.refSockaddrs().sin_port);
		print_status(ft_YELLOW, msg.str());
		
		struct sockaddr_in	client_address;
		long  				client_address_size = sizeof(client_address);
		int 				client_sock;
		Client  			new_client(server); 	// Instantiates a Client Object that stores a copy of this client's Server config to its member 

		client_sock = accept(server.readInSock(), (struct sockaddr *)&client_address,(socklen_t*)&client_address_size);
		if ( client_sock < 0 )
		{
			print_status(ft_RED, "::acceptConnection() Error! A call to Accept() failed");
			return ;
		}
		// adding this client's sock addr to rcvd_fds_tmp so that the if() condition
		// right after the line that called this function from runServers() can pick it up
		if (fcntl(client_sock, F_SETFL, O_NONBLOCK) < 0)
		{
			close(client_sock);
			print_status(ft_RED, "fcntl error");
			return ;
		}
		addFDToSet(client_sock, this->_received_fds);
		// Updating _biggest_fd to consider this client's outgoing Socket FD as well	
		new_client.setSocket(client_sock);					// storing this client's sock addr to new_client obj
		this->connected_clients[client_sock] = new_client;	// Adds this ClientsOutboundSockFD=>thisClient'sObject as a Key=>Value to connected_clients map
		// It might be necessary to store client_address to new_client.client_address at this point
		
		msg.str("");
		msg << "Client " << inet_ntoa(client_address.sin_addr)
			<< ":" << ntohs(client_address.sin_port)
			<< " Connected to "
			<< inet_ntoa(server.refSockaddrs().sin_addr) << ":"
			<< ntohs(server.refSockaddrs().sin_port)
			<< " Successfully!";
		print_status(ft_GREEN, msg.str());
	
	}

	// fd param is the outgoing Socket FD for reaching this
	// client, while client param is the Client object storing the
	// Server processing this client's request among other stuffs.
	//
	void ServerManager::readRequest(int fd, Client &client)
	{
		char buffer[BUFFER_SIZE + 1] = {'\0'};
		int bytes_read;
		
		// Reading Clients httpRequest details from their 
		// outbound socket addr FD into buffer
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		//std::cout << "Client request : \n" << buffer << std::endl;
		if (bytes_read == 0)
		{
			//TODO change msg
			//print_status(ft_GREEN, "Closing connection because no activity");
			removeFDToSet(fd, this->_received_fds);
			close(fd);
			this->connected_clients.erase(fd);
			return ;
		}
		else if (bytes_read < 0)
		{
			print_status(ft_RED, "Error on read ...");
			removeFDToSet(fd, this->_received_fds);
			close(fd);
			this->connected_clients.erase(fd);
			return ;
		}
		else
		{
			std::string request(buffer, bytes_read);
			
			client.updateTime();
			client.request.parse(request);
			memset(buffer, 0 , sizeof(buffer));
		}	
		if (client.request.parsingFinished() || client.request.getErrorCode() != NONE) // even on bad request server sends an answer
		{
			assign_server_for_response(client); // I think this is a duplicate action. i might be wrong though
			client.buildResponse(); //here
			removeFDToSet(fd, this->_received_fds);
			addFDToSet(fd, this->_write_fds);
		}

	}

	void ServerManager::sendResponce(int fd, Client &client)
	{

		long bytesSent;
		//std::cout << "server response: " << std::endl << client.response.refResponseCont() << std::endl;
		bytesSent = send(client.getSocket(), client.response.refResponseCont().data(), client.response.refResponseCont().size(), 0);
		if ( bytesSent >= 0 &&
				static_cast<long unsigned int>(bytesSent) == client.response.refResponseCont().size() )
				print_status(ft_GREEN, "Server Response sent to client");
		else
			print_status(ft_RED, "Error sending response to client");

		if (!client.request.keepAlive())
		{
			// print_status(ft_GREEN, "Closing connection to Client because response send");
			// closeConnectionToClient(fd);
			removeFDToSet(fd, this->_write_fds);
			addFDToSet(fd, this->_received_fds);
			client.request.clear();
			client.response.refResponseCont().clear();
		}
		else
		{
			removeFDToSet(fd, this->_write_fds);
			addFDToSet(fd, this->_received_fds);
			client.response.refResponseCont().clear();
			client.request.clear();
		}
	}

	void    ServerManager::assign_server_for_response(Client &client) // I thought the processing server has previously being assigned to this client.server() at the point of declaration in acceptConnection()?
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

	void ServerManager::addFDToSet(const int fd, fd_set &set)
	{
		FD_SET(fd, &set);
		if (fd > this->_biggest_fd)
			this->_biggest_fd = fd;
	}
	//TODO improve biggest fd search
	void ServerManager::removeFDToSet(const int fd, fd_set &set)
	{
		FD_CLR(fd, &set);
		if(fd == this->_biggest_fd)
			this->_biggest_fd--;
	}
} // namespace ft
