/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:06:35 by hrings            #+#    #+#             */
/*   Updated: 2023/03/03 19:50:40 by hrings           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "WebServer.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Server.hpp"

// a class that stores information about a client
class Client
{
	public:
		Client();
		Client(const Client &copy);
		Client(ft::Server &server);
		Client &operator=(const Client &assign);
		~Client();

		void	setSocket(int &fd);
        void	setAddress(sockaddr_in &addr);
        void	setServer(ft::Server &server);
		ft::Server getServer();
		int	getSocket();
		
		void buildResponse();
		Request getRequest() const;
		void updateTime();
		//TODO make request/response private?
		Request request;
		Response response;
	private:		
		ft::Server        server;
		int	client_socket;
		struct sockaddr_in client_address;
		time_t			time_of_last_msg;
		
		
};

#endif