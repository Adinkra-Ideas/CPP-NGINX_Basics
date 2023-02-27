#pragma once
#ifndef __CONFIGPARSER_HPP__
# define __CONFIGPARSER_HPP__

# include <iostream>
# include <vector>

# include "Server.hpp"

namespace ft {
	class ConfigParser {
	public:
		ConfigParser(std::vector<Server>& servers, const char *path);
		~ConfigParser();
		ConfigParser( const ConfigParser& other );

		ConfigParser& operator= ( const ConfigParser& other );


	private:
		ConfigParser();

	};


	//////////////////////// EXTERNAL HELPER FUNCTIONS BEGIN ////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	int		parse_config(const char *path);

}	// namespace ft

#endif
