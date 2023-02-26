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

	int		parse_config(const char *path) {
		std::ifstream 	_fin;

		_fin.open(path);
		if (  ! _fin.good() )
			return (1);
		
		// FOR TEST PURPOSE
		std::cout << ft_YELLOW << "config file named \"" << path << "\" opened successfully" << std::endl;
		
		return (0);
	}

}	// namespace ft
