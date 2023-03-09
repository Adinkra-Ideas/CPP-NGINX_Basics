#pragma once
#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include "WebServer.hpp"
# include "Server.hpp"
# include "Client.hpp"

# include <map>
# include <vector>
class Server;

# define MAX_QUEUE 512

namespace http {

	//the webserver
	//starts and runs the servers
	class ServerManager {
	public:
		typedef std::vector<http::Server> vector_of_servers;

		ServerManager( void );
		~ServerManager( void );
		ServerManager( const ServerManager& other );

		ServerManager& operator= ( const ServerManager& other );

		void    setupServers(std::vector<http::Server> servers);
		void    runServers( void );
		void acceptConnection(http::Server &server);
		void readRequest(int fd, Client &client);
		void sendResponce(int fd, Client &client);
		void    assign_server_for_response(Client &client);

	private:
		vector_of_servers			_servers;
		fd_set 						recive_fds;
		fd_set 						write_fds;
		int 						biggest_fd;
		std::map<int, http::Server> 	running_servers;
		std::map<int, Client> 		connected_clients;
	};

} // namespace ft

#endif
