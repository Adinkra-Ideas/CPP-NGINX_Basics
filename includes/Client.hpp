/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:06:35 by hrings            #+#    #+#             */
/*   Updated: 2023/03/21 20:52:19 by hrings           ###   ########.fr       */
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
		Client(http::Server &server);
		Client &operator=(const Client &assign);
		~Client();

		void			setSocket(int &fd);
        void			setAddress(sockaddr_in &addr);
        void			setServer(http::Server &server);
		http::Server& 	getServer();
		int				getSocket();
		
		void buildResponse();
		Request getRequest() const;
		void updateTime();
		time_t getupdateTime();
		//TODO make request/response private?
		Request request;
		http::Response response;					// This object receives the parsed requestHeader object through its method response.setRequest(request)
	private:		
		http::Server        server;				// Stores the Server that this client is connected to
		int					client_socket;		// Stores the outbound socked address FD where data intended for this client will be written
		struct sockaddr_in client_address;		// socket address created for client using accept()
		time_t			time_of_last_msg;		// Stores time when server received this client's request
		
		
};

#endif