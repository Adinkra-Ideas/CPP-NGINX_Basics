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
		std::vector<Listen> list_of_bind;
		int socked_fd;
		// we create registry to store each of the FDs allocated to each server context in config
		std::ofstream 		fout;
		fout.open("FD_Registry.txt", std::ios::out | std::ios::trunc );
		if ( ! fout.good() )
			exit_with_error("Unable to Create FD_Registry.txt File");

		for (std::vector<http::Server>::iterator iter = this->_servers.begin(); iter != this->_servers.end(); ++iter)
		{
			for (std::vector<Listen>::iterator it_li = iter->getListen().begin(); it_li != iter->getListen().end(); ++it_li)
			{
				
				if (std::find(list_of_bind.begin(), list_of_bind.end(), *it_li) == list_of_bind.end())
				{
					if ( (socked_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
					{
						exit_with_error("Socket() failed");
					}
					if ( socked_fd >= FD_SETSIZE ) {
						msg << "Allocated Socket FD " << iter->readInSock() 
						<< " is Greater than FD_SETSIZE value on Host Kernel"; 
						exit_with_error(msg.str());
					}
					if ( fcntl(iter->readInSock(), F_SETFL, O_NONBLOCK) == -1 ) {
						msg << "Setting Socket FD "
						<<  socked_fd
						<< " to Non-Block Mode Failed"; 
						exit_with_error(msg.str());
					}
					struct sockaddr_in address;
					memset(&address, 0, sizeof(address));
					address.sin_family = AF_INET;
					address.sin_port = htons(it_li->port);
					address.sin_addr.s_addr = inet_addr(it_li->ip.c_str());
					int optval = 1;
					socklen_t optlen = sizeof(optval);
					if (setsockopt(socked_fd, SOL_SOCKET, SO_REUSEADDR, &optval, optlen) < 0 )
					{
						msg << "setsockopt failed on " << socked_fd;
						exit_with_error(msg.str());
					}
					if(setsockopt(socked_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
						msg << "setsockopt failed on " << socked_fd;
						exit_with_error(msg.str());
   					}
					if (bind(socked_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
						msg << "bind went wrong for host " << it_li->ip << ", " << it_li->port;
						exit_with_error(msg.str());
					}
					addFDToSet(socked_fd, this->_received_fds);
					this->_running_servers[socked_fd] = *it_li;
					fout << socked_fd << "\r\n";
					if ( listen(socked_fd, MAX_QUEUE) ) {
						msg << "Socket listening on Sock FD " << socked_fd
						<< " failed";
						exit_with_error(msg.str());
					}
					std::ostringstream	msg2;
					list_of_bind.push_back(*it_li);
					msg2 << "Connected to " << it_li->ip << ":" << it_li->port;
					print_status(ft_GREEN, msg2.str());
				}
			}
			fout << iter->readInSock() << "\r\n";
		}
		if (list_of_bind.empty())
			exit_with_error("Failed to start an Server");
		fout.close();
		ft::initSignal();
	}

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
				// check if coonection ...
				if( FD_ISSET(i, &_received_fds_tmp) && this->_running_servers.count(i) )
					acceptConnection(i);

				// Checking if acceptConnection() has already accepted this FD successfully
				// and initialized a connected_clients Map element for it
				// but here, first i is server FD while second i is client FD. How possible?
				// else if ( FD_ISSET(i, &_received_fds_tmp) && this->connected_clients.count(i) )
				else if ( FD_ISSET(i, &_received_fds_tmp) && this->connected_clients.count(i) )
					readRequest(i, this->connected_clients[i]);
				
				// We're here. But first, we need to complete understanding what's going on in readRequest(). the request and response objects
				else if (FD_ISSET(i, &_write_fds_tmp) && this->connected_clients.count(i))
					sendResponce(i, this->connected_clients[i]);
			}
			checkTimeout();
			// usleep(500);
		}	
	}
	void ServerManager::checkTimeout()
	{
		for(std::map<int, Client>::iterator iter = this->connected_clients.begin(); iter != this->connected_clients.end(); ++iter)
		{
			if (difftime(time(NULL), iter->second.getupdateTime()) > TIMEOUTTIME && iter->second.getRequest().has_request())
			{
				iter->second.request.setStatusCode(REQUESTTIMEOUT);
				iter->second.buildResponse();
				removeFDToSet(iter->first, this->_received_fds);
				addFDToSet(iter->first, this->_write_fds);
				print_status(ft_GREEN, "Timeout Closing connection to Client because of timeout");
				iter->second.updateTime();
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
	void ServerManager::acceptConnection(int fd)
	{
		std::ostringstream	msg;

		msg << "New Client is Connecting to Socket Address "
			<< this->_running_servers[fd].ip << ":" 
			<< this->_running_servers[fd].port;
		print_status(ft_YELLOW, msg.str());
		
		struct sockaddr_in	client_address;
		long  				client_address_size = sizeof(client_address);
		int 				client_sock;
		Client  			new_client(this->_running_servers[fd], client_address); 	// Instantiates a Client Object that stores a copy of this client's Server config to its member 

		client_sock = accept(fd, (struct sockaddr *)&client_address,(socklen_t*)&client_address_size);
		if ( client_sock < 0 )
		{
			print_status(ft_RED, "::acceptConnection() Error! A call to Accept() failed");
			return ;
		}
		new_client.request.set_client_ip(new_client.get_Ip_Address());
		int optval = 1;
		socklen_t optlen = sizeof(optval);
		if(setsockopt(client_sock, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
			print_status(ft_RED, "setsockopt failed ");
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
			<< " Connected Successfully!";
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
		//std::cout << "client request:" << std::endl << buffer << std::endl;
		if (bytes_read == 0)
		{
			print_status(ft_GREEN, "Closing connection because no activity");
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
			client.setServer(assign_server_for_response(client));
			client.buildResponse();
			removeFDToSet(fd, this->_received_fds);
			addFDToSet(fd, this->_write_fds);
		}

	}

	void ServerManager::sendResponce(int fd, Client &client)
	{

		int bytesSent;
		bytesSent = send(client.getSocket(), client.response.refResponseCont().c_str() + client.response.get_bytesend(),
			 client.response.refResponseCont().size() - client.response.get_bytesend(), 0);
		if ( bytesSent > 0)
		{
			client.response.set_bytesend(client.response.get_bytesend() + bytesSent);
		}
		else if (bytesSent == 0)
		{
			print_status(ft_GREEN, "Closing connection because no activity");
			removeFDToSet(fd, this->_write_fds);
			close(fd);
			this->connected_clients.erase(fd);
			return ;
		}
		else
		{
			print_status(ft_RED, "Error sending response to client");
			removeFDToSet(fd, this->_write_fds);
			close(fd);
			this->connected_clients.erase(fd);
			return ;
		}
			
		if (static_cast<size_t>(client.response.get_bytesend()) == client.response.refResponseCont().size())
		{
			print_status(ft_GREEN, "Server Response sent to client");
			//always keep the connection alive
			removeFDToSet(fd, this->_write_fds);
			addFDToSet(fd, this->_received_fds);
			std::string("").swap(client.response.refResponseCont());
			//client.response.refResponseCont().clear();
			client.response.set_bytesend(0);
			client.request.clear();
		}
	}

	Server&    ServerManager::assign_server_for_response(Client &client)
	{
		std::vector<Server*> possible_servers;
		for (std::vector<http::Server>::iterator iter = this->_servers.begin(); iter != this->_servers.end(); ++iter)
		{
			for (std::vector<Listen>::iterator it_li = iter->getListen().begin(); it_li != iter->getListen().end(); ++it_li)
			{
				if (*it_li == client.getListen())
				{
					possible_servers.push_back(&(*iter));	
				}
			}
		}
		if (possible_servers.size() == 1)
			return *possible_servers.front();
		else
		{
			std::string req_server_name = client.getRequest().headers["host"].substr(0, client.getRequest().headers["host"].find(':'));
			for (std::vector<Server*>::iterator it = possible_servers.begin(); it != possible_servers.end(); ++it)
			{
				if (!(**it).readName().compare(req_server_name))
				{
					return **it;
				}
					
			}
			return *(possible_servers.front());
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

} // namespace http
