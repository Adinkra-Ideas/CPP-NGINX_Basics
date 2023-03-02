#pragma once
#ifndef __CONFIGPARSER_HPP__
# define __CONFIGPARSER_HPP__

# include <iostream>
# include <vector>
# include <sstream>
# include <cstring>

# include "Server.hpp"
# include "Location.hpp"
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
		bool		parse_server(std::vector<Server>& servers,  const std::string& block);
		std::size_t	calculate_closing_brace_pos(const std::string& block);
		bool		parse_one_server(std::vector<Server>& servers, std::string block);
		bool		parse_location(std::vector<Location>& locations, std::string& context);
		void		parse_to_str(const std::string& str, const char *name, const std::string& context);
		void		parse_to_vect(std::vector<std::string>& methods, const std::string& context);
		void		setup_server_host(struct sockaddr_in& sockAddr, const std::string& listen);
	};

}	// namespace ft

#endif
