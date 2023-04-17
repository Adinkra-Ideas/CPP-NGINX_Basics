/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:12:03 by hrings            #+#    #+#             */
/*   Updated: 2023/04/17 19:11:21 by hrings           ###   ########.fr       */
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
		client_address(copy.client_address)

{
}

Client::Client(http::Server &server)
{
	// this->request = NULL;
	// this->response = NULL;
	this->server = server;
	size_t max_body = server.readMaxBody();
	if (max_body)
		this->request.set_max_body_size(max_body);
	else
		this->request.set_max_body_size(2147483647);
}

Client &Client::operator=(const Client &assign)
{
	if (this != &assign)
	{
		this->request = assign.request;
		this->response = assign.response;
		this->server = assign.server;
		this->client_socket = assign.client_socket;
		this->client_address = assign.client_address;
		this->time_of_last_msg = assign.time_of_last_msg;
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
void	Client::setAddress(sockaddr_in &addr)
{
	this->client_address = addr;
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
