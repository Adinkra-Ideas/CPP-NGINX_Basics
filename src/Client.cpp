/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:12:03 by hrings            #+#    #+#             */
/*   Updated: 2023/04/21 18:23:56 by hrings           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(): time_of_last_msg(0)
{
	// this->request = NULL;
	// this->response = NULL;
}

Client::Client(const Client &copy) :
		request(copy.request),
		response(copy.response),
		server(copy.server),
		client_socket(copy.client_socket),
		_clientAddrs(copy._clientAddrs),
		time_of_last_msg(copy.time_of_last_msg),
		_listen(copy._listen)
		
{
}

Client::Client(http::Listen listen, struct	sockaddr_in	clientAddrs) : 
	_clientAddrs(clientAddrs), _listen(listen)
{
	// this->request = NULL;
	// this->response = NULL;
}
Client &Client::operator=(const Client &assign)
{
	if (this != &assign)
	{
		this->request = assign.request;
		this->response = assign.response;
		this->server = assign.server;
		this->client_socket = assign.client_socket;
		this->_clientAddrs = assign._clientAddrs;
		this->time_of_last_msg = assign.time_of_last_msg;
		this->_listen = assign._listen;
	}
	return (*this);
}
Client::~Client()
{
	
}

void	Client::setSocket(int &fd)
{
	this->client_socket = fd;
}
std::string	Client::get_Ip_Address()
{
	return (inet_ntoa(_clientAddrs.sin_addr));
}

void	Client::setServer(http::Server &server)
{
	this->server = server;
}

Request Client::getRequest() const
{
	return (this->request);
}

int	Client::getSocket()
{
	return (this->client_socket);
}

http::Listen Client::getListen()
{
	return (this->_listen);	
}
void Client::updateTime()
{
	this->time_of_last_msg = time(NULL);
}

time_t Client::getupdateTime()
{
	return (this->time_of_last_msg);
}

http::Server& Client::getServer()
{
	return (this->server);
}

void Client::buildResponse() // construct a dynamic http Header response for this client
{
	this->response.setRequest(this->request); // copying request to client
	this->response.setServer(this->server);
	this->response.buildResponse();				//here
}
