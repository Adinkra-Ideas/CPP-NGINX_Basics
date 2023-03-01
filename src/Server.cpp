# include "../includes/Server.hpp"

namespace ft {

	Server::Server( void ) {}

	Server::~Server( void ) {}

	Server::Server( const Server& other ) : _socketAddress(), _server_name(other._server_name),
						_root(other._root), _max_body(other._max_body),
						_error_page(other._error_page), _locations(other._locations) {
		_socketAddress.sin_family = AF_INET;
		_socketAddress.sin_port = other._socketAddress.sin_port;
		_socketAddress.sin_addr.s_addr = other._socketAddress.sin_addr.s_addr;		
	}

	Server& Server::operator= ( const Server& other ) {
		(void)other;
		return *this;
	}

	// ****************	CONSTRUCTORS && OPERATORS ENDS	**********************
	//////////////////////////////////////////////////////////////////////////
	// ******************* 		METHODS BEGINS			**********************
	std::vector<Location>& Server::refLocations( void ) { return _locations; }

}	// namespace ft
