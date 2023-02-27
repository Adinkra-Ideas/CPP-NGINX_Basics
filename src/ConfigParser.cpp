# include "../includes/ConfigParser.hpp"

namespace ft {

	ConfigParser::ConfigParser() {}

	ConfigParser::ConfigParser(std::vector<Server>& servers, const char *path) {
		parse_config(path);
		(void) servers;

	}

	ConfigParser::~ConfigParser() {}

	ConfigParser::ConfigParser( const ConfigParser& other ) {
		(void) other;
	}

	ConfigParser& ConfigParser::operator= ( const ConfigParser& other ) {
		(void) other;
		return *this;
	}


	//////////////////////// EXTERNAL HELPER FUNCTIONS BEGIN ////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	int		parse_config(const char *path) {
		std::ifstream 	_fin;

		_fin.open(path);
		if ( ! _fin.good() )
			exit_with_error(ft_RED, "Failed To Read From Config File");
		
		// FOR TEST PURPOSE
		std::cout << ft_YELLOW << path << " file opened successfully" << std::endl;
		
		return (0);
	}



}	// namespace ft