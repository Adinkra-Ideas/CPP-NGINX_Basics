# include "../includes/Server.hpp"

namespace http {
	Server::Server( void ) : _port(), _ip(), _sockAddrs(), _sockAddrs_len(),
						 _in_sock(), _name(), _root(),
						 _max_body(MAXBODYSIZE), _error_page(), _locations(), cgi() {}

	Server::~Server( void ) {}

	Server::Server( const Server& other ) : _port(other._port), _ip(other._ip),
						_sockAddrs(), _sockAddrs_len(), _in_sock(),
						_name(other._name), _root(other._root), _max_body(other._max_body),
						_error_page(other._error_page), _locations(other._locations),
						cgi(other.cgi)
	{
		_sockAddrs.sin_family = other._sockAddrs.sin_family;
		_sockAddrs.sin_port = other._sockAddrs.sin_port;
		_sockAddrs.sin_addr.s_addr = other._sockAddrs.sin_addr.s_addr;
		_sockAddrs_len = other._sockAddrs_len;
	}

	Server& Server::operator= ( const Server& other ) {
		if (this != &other)
		{
			this->_port = other._port;
			this->_ip = other._ip;
			this->_sockAddrs = other._sockAddrs;
			this->_sockAddrs_len = other._sockAddrs_len;
			this->_in_sock = other._in_sock;
			this->_name = other._name;
			this->_root = other._root;
			this->_max_body = other._max_body;
			this->_error_page = other._error_page;
			this->_locations = other._locations;
			this->cgi = other.cgi;
		}
		return *this;
	}

	// ****************	CONSTRUCTORS && OPERATORS ENDS	**********************
	//////////////////////////////////////////////////////////////////////////
	// ***********		READING AND WRITING METHODS BEGINS********************
	void	Server::writePort(const int& port) { _port = port; }
	const int&	Server::readPort( void ) { return _port; }

	void	Server::writeIp(const std::string& listen) { _ip = listen; }
	const std::string&	Server::readIp( void ) { return _ip; }

	void	Server::writeSockAddrLen(const unsigned int& len) { _sockAddrs_len = len; }
	const unsigned int&	Server::readSockAddrLen( void ) { return _sockAddrs_len; }

	void	Server::writeInSock(const int& fd) { _in_sock = fd; }
	const int&	Server::readInSock( void ) { return _in_sock; }

	void	Server::writeName(const std::string& name) { _name = name; }
	const std::string&	Server::readName( void ) { return _name; }

	void	Server::writeRoot(const std::string& root) { _root = root; }
	const std::string&	Server::readRoot( void ) { return _root; }

	void	Server::writeMaxBody(const std::size_t& max_body) { _max_body = max_body; }
	const std::size_t&	Server::readMaxBody( void ) { return _max_body; }

	void	Server::writeErrorPage(const std::string& error_page) { _error_page = error_page; }
	const std::string&	Server::readErrorPage( void ) { return _error_page; }

	void Server::setCgi(const std::map<std::string, std::pair<std::string, std::string> > &input) {this->cgi = input;}
	std::map<std::string, std::pair<std::string, std::string> >& Server::getCgi() { return this->cgi;}
	
	// ***********		READING AND WRITING METHODS ENDS *********************
	//////////////////////////////////////////////////////////////////////////
	// **** 	Returning Reference Address of Member Objects Begins	******
	std::vector<Location>& Server::refLocations( void ) { return _locations; }
	struct	sockaddr_in& Server::refSockaddrs( void ) { return _sockAddrs; }

	// **** 	Returning Reference Address of Member Objects Ends	******
	//////////////////////////////////////////////////////////////////////
	// ***********		Initializing the Server Begins 		**************

	// **************************************************
	// Creates a socket and store the FD to _in_sock	*
	// If successful, Bind() the _in_sock to _sockAddrs	*
	// which already has the IP:HOST stored				*
	// **************************************************
	void	Server::bindServerSockAddr() {
		std::ostringstream msg;

		if ( (_in_sock = socket(AF_INET, SOCK_STREAM, 0) )  == -1 ) {
			msg << "Cannot create socket for host " << _ip;
			exit_with_error(msg.str());
		}
		if (bind(_in_sock, (struct sockaddr *)&_sockAddrs, _sockAddrs_len) == -1) {
			msg << "bind went wrong for host " << _ip;
			exit_with_error(msg.str());
		}
		int optval = 1;
		socklen_t optlen = sizeof(optval);
		if(setsockopt(_in_sock, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
			msg << "setsockopt failed " << _ip;
			exit_with_error(msg.str());
   		}

		msg << "Socket FD " << _in_sock
			<< " bounded successfully with Socket Address "
			<< inet_ntoa(_sockAddrs.sin_addr) << ":"
			<< ntohs(_sockAddrs.sin_port);
		print_status(ft_GREEN, msg.str());
	}

	// **************************************************
	// Starts listen() for our socket address FD stored	*
	// already in _in_sock								*
	// **************************************************
	void	Server::startListen( const int& max_queue) {
		std::ostringstream msg;

		if ( listen(_in_sock, max_queue) ) {
			msg << "Socket listening on Sock FD " << _in_sock
				<< " failed";
			exit_with_error(msg.str());
		}

		msg << "Socket Address " << inet_ntoa(_sockAddrs.sin_addr)
			<< ":" << ntohs(_sockAddrs.sin_port) << " With FD "
			<< _in_sock << " is Now Listening! (Max Queue == "
			<< max_queue << ")" ;
		print_status(ft_GREEN, msg.str());
	}

}	// namespace ft
