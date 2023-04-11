#pragma once
#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include <fstream>
# include <map>
# include <sys/socket.h>
# include <arpa/inet.h>

# include "Location.hpp"
# include "WebServer.hpp"
# include "utils.hpp"

namespace http {

	class Server {
	public:
		// ****** Constructors and Destructor **********
		Server( void );
		~Server( void );
		Server( const Server& other );
		
		// ************* Operators *********************
		Server& operator= ( const Server& other );

		// ************* Reading & Writing Operations **********
		void				writePort(const int& port);
		const int&			readPort( void );
		void				writeIp(const std::string& listen);
		const std::string&	readIp( void );
		void				writeSockAddrLen(const unsigned int& port);
		const unsigned int&	readSockAddrLen( void );
		void				writeInSock(const int& fd);
		const int&			readInSock( void );
		void				writeOutSock(const int& fd);
		const int&			readOutSock( void );
		void				writeName(const std::string& name);
		const std::string&	readName( void );
		void				writeRoot(const std::string& root);
		const std::string&	readRoot( void );
		void				writeMaxBody(const std::size_t& max_body);
		const std::size_t&	readMaxBody( void );
		void				writeErrorPage(const std::string& error_page);
		const std::string&	readErrorPage( void );
		void setCgi(const std::map<std::string, std::pair<std::string, std::string> >&str);
		std::map<std::string, std::pair<std::string, std::string> >& getCgi();

		// ***** Returning Reference Address of Member Objects *********
		std::vector<Location>&	refLocations( void );
		struct	sockaddr_in&	refSockaddrs( void );

		// ****** The Rest  *******
		void	bindServerSockAddr( void );
		void	startListen( const int& max_queue );

	private:		
		int						_port;
		std::string				_ip;
		struct	sockaddr_in		_sockAddrs;
		unsigned int			_sockAddrs_len;			// Stores the sizeof(_sockAddrs)

		int						_in_sock;				// for storing listening socket

		std::string				_name;					// (OPTIONAL)	Set the server_names or not
		std::string				_root;					// (MANDATORY)	Define a directory or a file from where the returned file should be searched
		std::size_t				_max_body;				// (OPTIONAL)	Limit client body size
		std::string				_error_page;			// (MANDATORY)	Setup default directory where error pages will be fetched from. 
		std::vector<Location>	_locations;				// (OPTIONAL)	For setting multiple single-level-deep locations for this Server instance (if present in config file)
		std::map<std::string, std::pair<std::string, std::string> >	cgi; //(OPTIONAL) list of extension with what method they allowed and the name of the exe
	
	};

}	// namespace ft

#endif
