# include "../includes/Server.hpp"

namespace ft {

	Server::Server( void ) : _port(), _ip(), _sockAddrs(), _sockAddrs_len(),
						 _in_sock(), _out_sock(), _name(), _root(),
						 _max_body(), _error_page(), _locations() {}

	Server::~Server( void ) {}

	Server::Server( const Server& other ) : _port(other._port), _ip(other._ip),
						_sockAddrs(), _sockAddrs_len(), _in_sock(), _out_sock(),
						_name(other._name), _root(other._root), _max_body(other._max_body),
						_error_page(other._error_page), _locations(other._locations){
		_sockAddrs.sin_family = other._sockAddrs.sin_family;
		_sockAddrs.sin_port = other._sockAddrs.sin_port;
		_sockAddrs.sin_addr.s_addr = other._sockAddrs.sin_addr.s_addr;
		_sockAddrs_len = other._sockAddrs_len;
	}

	Server& Server::operator= ( const Server& other ) {
		(void)other;
		return *this;
	}

	// ****************	CONSTRUCTORS && OPERATORS ENDS	**********************
	//////////////////////////////////////////////////////////////////////////
	// ******************* 		METHODS BEGINS			**********************
	void	Server::writePort(const int& port) { _port = port; }
	const int&	Server::readPort( void ) { return _port; }

	void	Server::writeIp(const std::string& listen) { _ip = listen; }
	const std::string&	Server::readIp( void ) { return _ip; }

	void	Server::writeSockAddrLen(const unsigned int& len) { _sockAddrs_len = len; }
	const unsigned int&	Server::readSockAddrLen( void ) { return _sockAddrs_len; }

	void	Server::writeInSock(const int& fd) { _in_sock = fd; }
	const int&	Server::readInSock( void ) { return _in_sock; }

	void	Server::writeOutSock(const int& fd) { _out_sock = fd; }
	const int&	Server::readOutSock( void ) { return _out_sock; }

	void	Server::writeName(const std::string& name) { _name = name; }
	const std::string&	Server::readName( void ) { return _name; }

	void	Server::writeRoot(const std::string& root) { _root = root; }
	const std::string&	Server::readRoot( void ) { return _root; }

	void	Server::writeMaxBody(const std::string& max_body) { _max_body = max_body; }
	const std::string&	Server::readMaxBody( void ) { return _max_body; }

	void	Server::writeErrorPage(const std::string& error_page) { _error_page = error_page; }
	const std::string&	Server::readErrorPage( void ) { return _error_page; }

	std::vector<Location>& Server::refLocations( void ) { return _locations; }

	struct	sockaddr_in& Server::refSockaddrs( void ) { return _sockAddrs; }

}	// namespace ft
