#pragma once
#ifndef __CONFIGPARSER_HPP__
# define __CONFIGPARSER_HPP__

# include <iostream>
# include <vector>
# include <sstream>
# include <cstring>

# include "Server.hpp"
# include "utils.hpp"

namespace ft {
	class ConfigParser {
	public:
		ConfigParser(std::vector<Server>& servers, const char *path);
		~ConfigParser( void );
		ConfigParser( const ConfigParser& other );

		ConfigParser& operator= ( const ConfigParser& other );



	private:
		ConfigParser( void );

		void		parse_config( std::vector<Server>& servers, const char *path );
		void		erase_comments( std::string& line );
		bool		verif_and_parse_one_server(std::vector<Server>& servers,  const std::string& block);
		std::size_t	calculate_closing_brace_pos(const std::string& block);
		bool		configure_one_server(std::vector<Server>& servers, std::string block);


	};

}	// namespace ft

#endif
