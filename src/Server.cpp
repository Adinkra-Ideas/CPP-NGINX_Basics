# include "../includes/Server.hpp"

namespace ft {

	Server::Server( void ) {}

	Server::~Server( void ) {}

	Server::Server( const Server& other ) : _listen(other._listen), _name(other._name),
						_root(other._root), _max_body(other._max_body),
						_error_page(other._error_page), _locations(other._locations){}

	Server& Server::operator= ( const Server& other ) {
		(void)other;
		return *this;
	}

	// ****************	CONSTRUCTORS && OPERATORS ENDS	**********************
	//////////////////////////////////////////////////////////////////////////
	// ******************* 		METHODS BEGINS			**********************
	void	Server::writeListen(const std::string& listen) { _listen = listen; }
	const std::string&	Server::readListen( void ) { return _listen; }

	void	Server::writeName(const std::string& name) { _name = name; }
	const std::string&	Server::readName( void ) { return _name; }

	void	Server::writeRoot(const std::string& root) { _root = root; }
	const std::string&	Server::readRoot( void ) { return _root; }

	void	Server::writeMaxBody(const std::string& max_body) { _max_body = max_body; }
	const std::string&	Server::readMaxBody( void ) { return _max_body; }

	void	Server::writeErrorPage(const std::string& error_page) { _error_page = error_page; }
	const std::string&	Server::readErrorPage( void ) { return _error_page; }

	std::vector<Location>& Server::refLocations( void ) { return _locations; }

}	// namespace ft
