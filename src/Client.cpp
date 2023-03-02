/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:12:03 by hrings            #+#    #+#             */
/*   Updated: 2023/03/02 11:51:29 by hrings           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	this->request = NULL;
	this->response = NULL;
}

Client::Client(const Client &copy) :
		request(copy.request),
		response(copy.response),
		server(copy.server),
		client_socket(copy.client_socket),
		client_address(copy.client_address)

{
}

Client::Client(ServerConfig &server)
{
	this->request = NULL;
	this->response = NULL;
	this->server = server;
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
void	Client::setServer(ServerConfig &server)
{
	this->server = server;
}

Request *Client::getRequest()
{
	if (!this->request)
		this->request = new Request();
	return (this->request);
}