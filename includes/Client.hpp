/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:06:35 by hrings            #+#    #+#             */
/*   Updated: 2023/04/21 18:18:53 by hrings           ###   ########.fr       */
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
		Client(http::Listen listen, struct	sockaddr_in	clientAddrs);
		Client &operator=(const Client &assign);
		~Client();

		void			setSocket(int &fd);
        std::string		get_Ip_Address();
        void			setServer(http::Server &server);
		http::Server& 	getServer();
		int				getSocket();
		http::Listen	getListen();
		void buildResponse();
		Request getRequest() const;
		void updateTime();
		time_t getupdateTime();
		Request request;
		http::Response response;					// This object receives the parsed requestHeader object through its method response.setRequest(request)
	private:		
		http::Server        server;				// Stores the Server that this client is connected to
		int					client_socket;		// Stores the outbound socked address FD where data intended for this client will be written
		struct	sockaddr_in		_clientAddrs;		// socket address created for client using accept()
		time_t			time_of_last_msg;		// Stores time when server received this client's request
		http::Listen _listen;

		
};

#endif
