#pragma once
#ifndef __CONFIGPARSER_HPP__
# define __CONFIGPARSER_HPP__

// Max value for a localhost port number == max 16 bit int value (65535)
# define MAX_16_BIT 65535

// Minimum len for a local IP is 7 chars (0.0.0.0)
# define MIN_HOST_LEN 7

// for converting z.B. 1 KBi = 1024 Bytes
# define INTtoKBi 1024

# include <iostream>
# include <vector>
# include <sstream>
# include <cstring>
# include <cstdlib>

# include "Server.hpp"
# include "Location.hpp"
# include "utils.hpp"

namespace http {

	class ConfigParser {
	public:
		// ****** Constructors and Destructor **********
		ConfigParser(std::vector<Server>& servers, const char *path);
		~ConfigParser( void );
		ConfigParser( const ConfigParser& other );

		// ************* Operators *********************
		ConfigParser& operator= ( const ConfigParser& other );

	private:
		ConfigParser( void );

		// ******************* Private Methods  *******************************
		void		parse_config( std::vector<Server>& servers, const char *path );
		void		erase_comments( std::string& line );
		bool		find_server_context(std::vector<Server>& servers,  const std::string& block);
		std::size_t	calculate_closing_brace_pos(const std::string& block);
		bool		parse_one_server(std::vector<Server>& servers, std::string block);
		bool		parse_one_location(std::vector<Location>& locations, std::string& context);
		void		parse_to_str(const std::string& str, const char *name, const std::string& context);
		void		parse_to_vect(std::vector<std::string>& methods, const std::string& context);
		void		setup_server_host(struct sockaddr_in& _sockAddrs, const unsigned int& sockAddrs_len,
									const std::string& ip, const int& port);
		void		max_body_to_int(const std::size_t& max_body, std::string& max_body_tmp);
		void 		parse_for_cgi(std::map<std::string, std::pair<std::string, std::string> > &cgi_map, std::string &context);
		std::vector<std::string> split_string(std::string str);
	};	

}	// namespace ft

#endif
