#pragma once
#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

# include <iostream>
# include <string>
# include <vector>

namespace http {

	class Location {
	public:
		Location( void );
		~Location( void );
		Location( const Location& other );

		Location& operator= ( const Location& other );

		std::vector<std::string>& refMethods( void );

		void				writePath(const std::string& path);
		const std::string&	readPath( void );
		void				writeRoot(const std::string& root);
		const std::string&	readRoot( void );
		void				writeIndex(const std::string& index);
		const std::string&	readIndex( void );
		void				writeAutoind(const std::string& autoind);
		const std::string&	readAutoind( void );
		void				writeExec(const std::string& exec);
		const std::string&	readExec( void );
		void				writeRewrite(const std::string& rewrite);
		const std::string&	readRewrite( void );
		void				writeUploads(const std::string& uploads);
		const std::string&	readUploads( void );

	private:
		std::string					_path;		// (MANDATORY)	could be something like "/" or maybe "/cgi-bin"
		std::vector<std::string>	_methods;	// (OPTIONAL)	Define a list of accepted HTTP methods for the route
		std::string					_root;		// (OPTIONAL)	Define a directory or a file from where the file should be searched
		std::string					_index;		// (MANDATORY)	Set a default file to answer if the request is a directory.
		std::string					_autoindex;	// (OPTIONAL)	Turn on or off directory listing
		std::string					_exec;		// (OPTIONAL)	Execute CGI based on certain file extension (for example .php)
		std::string					_rewrite;	// (OPTIONAL)	Define a HTTP redirection
		std::string					_uploads;	// (OPTIONAL)	Define the directory where uploaded files will be stored
	};

}	// namespace ft

#endif
