/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:06:35 by hrings            #+#    #+#             */
/*   Updated: 2023/03/02 11:39:47 by hrings           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "WebServer.hpp"
# include "Request.hpp"
# include "Response.hpp"

// a class that stores information about a client
class Client
{
	public:
		Client();
		Client(const Client &copy);
		Client(ServerConfig &server);
		Client &operator=(const Client &assign);
		~Client();

		void	setSocket(int &fd);
        void	setAddress(sockaddr_in &addr);
        void	setServer(ServerConfig &server);
		Request* getRequest();
	private:
		Request *request;
		Response *response;
		ServerConfig        server;
		int	client_socket;
		struct sockaddr_in client_address;
		
		
};

#endif