#pragma once
#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include "ConfigParser.hpp"
# include "WebServer.hpp"
# include "Server.hpp"
# include "Client.hpp"

# include <map>
# include <vector>
# include <sstream>
# include <errno.h>
# include <fstream>

// Max Queue used By listen()
# define MAX_QUEUE 512

namespace http {

	//the webserver
	//starts and runs the servers
	class ServerManager {
	public:
		typedef http::ConfigParser parser_function_object;

		// ****** Constructors and Destructor **********
		ServerManager( void );
		~ServerManager( void );
		ServerManager( const ServerManager& other );

		// ************* Operators *********************
		ServerManager& operator= ( const ServerManager& other );

		// ****** Managing the Servers  *******
		void	parseConfig( const char *path );
		void    setupServers( void );
		void    runServers( void );
		void	acceptConnection(http::Server &server);
		void	readRequest(int fd, Client &client);
		void	sendResponce(int fd, Client &client);
		void    assign_server_for_response(Client &client);

	private:
		std::vector<http::Server>		_servers;			// Each element in this _servers vector holds a server config
		fd_set 							_received_fds;		// For each Server config stored in _servers vector, the socket Address FD is added to this FD sets
		fd_set 							_write_fds;
		int 							_biggest_fd;		// stores the FD of _running_servers with the greatest value
		std::map<int, http::Server> 	_running_servers;	// mapped as foreach Running Server (Server._in_sock=>Server)
		std::map<int, Client> 		connected_clients;

		void checkTimeout();
		void addFDToSet(const int fd, fd_set &set);
		void removeFDToSet(const int fd, fd_set &set);
		void closeConnectionToClient(const int fd);
	};

} // namespace ft

#endif
