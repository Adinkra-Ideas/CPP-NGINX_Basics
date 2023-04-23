# include "../includes/Server.hpp"

namespace http {
	Server::Server( void ) : _in_sock(), _name(), _root(), _max_body(),
								_error_page(), _locations(), cgi() {}

	Server::~Server( void ) {}

	Server::Server( const Server& other ) : listen_to(other.listen_to),
						 _in_sock(), _name(other._name),
						 _root(other._root), _max_body(other._max_body),
						_error_page(other._error_page), _locations(other._locations),
						cgi(other.cgi)
	{
		// _sockAddrs.sin_family = other._sockAddrs.sin_family;
		// _sockAddrs.sin_port = other._sockAddrs.sin_port;
		// _sockAddrs.sin_addr.s_addr = other._sockAddrs.sin_addr.s_addr;
		// _sockAddrs_len = other._sockAddrs_len;
	}

	Server& Server::operator= ( const Server& other ) {
		if (this != &other)
		{
			this->listen_to = other.listen_to;
			// this->_port = other._port;
			// this->_ip = other._ip;
			// this->_sockAddrs = other._sockAddrs;
			// this->_sockAddrs_len = other._sockAddrs_len;
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
	
	std::vector<Listen>& Server::getListen(void) {return this->listen_to;}
	// ***********		READING AND WRITING METHODS ENDS *********************
	//////////////////////////////////////////////////////////////////////////
	// **** 	Returning Reference Address of Member Objects Begins	******
	std::vector<Location>& Server::refLocations( void ) { return _locations; }

	struct	sockaddr_in& Server::refSockaddrs( void ) { return _sockAddrs; }

}	// namespace ft
