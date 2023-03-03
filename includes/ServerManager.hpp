#pragma once
#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include "WebServer.hpp"
# include "Server.hpp"
# include "Client.hpp"

# include <map>
class Server;

# define MAX_QUEUE 20

//the webserver
//starts and runs the servers
class ServerManager {
public:
	ServerManager( void );
	~ServerManager( void );
	ServerManager( const ServerManager& other );

	ServerManager& operator= ( const ServerManager& other );

	void    setupServers(std::vector<ft::Server> servers);
	void    runServers( void );
	void acceptConnection(ft::Server &server);
	void readRequest(int fd, Client &client);
	void sendResponce(int fd, Client &client);
	void    assign_server_for_response(Client &client);

private:
	std::vector<ft::Server>	_servers;
	fd_set recive_fds;
	fd_set write_fds;
	int biggest_fd;
	std::map<int, ft::Server> running_servers;
	std::map<int, Client> connected_clients;
};

#endif
