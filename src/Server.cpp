# include "../includes/Server.hpp"

namespace ft {

	Server::Server( void ) {}

	Server::~Server( void ) {}

	Server::Server( const Server& other ) {
		(void)other;
	}

	Server& Server::operator= ( const Server& other ) {
		(void)other;
		return *this;
	}

	//////////////////////// EXTERNAL HELPER FUNCTIONS BEGIN ////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	void	exit_with_error(const char *color, const std::string& msg) {
		std::cout << color << msg << ft_RESET_COLOR << std::endl;
		std::exit(1);
	}



}	// namespace ft
