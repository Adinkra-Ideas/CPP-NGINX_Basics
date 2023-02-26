#pragma once
#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

# include <iostream>
# include <string>
# include <vector>

namespace ft {

	class Location {
	public:
		Location( void );
		~Location( void );
		Location( const Location& other );

		Location& operator= ( const Location& other );

	private:
		std::string					_path;		// (MANDATORY)	could be something like "/" or maybe "/cgi-bin"
		std::vector<std::string>	_methods;	// (OPTIONAL)	Define a list of accepted HTTP methods for the route
		std::string					_root;		// (MANDATORY)	Define a directory or a file from where the file should be searched
		std::string					_index;		// (MANDATORY)	Set a default file to answer if the request is a directory.
		std::string					_autoindex;	// (OPTIONAL)	Turn on or off directory listing
		std::string					_exec;		// (OPTIONAL)	Execute CGI based on certain file extension (for example .php)
	};

}	// namespace ft

#endif
