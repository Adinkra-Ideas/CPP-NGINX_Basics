#pragma once
#ifndef __UTILS_HPP__
# define __UTILS_HPP__

# define ft_RESET_COLOR	"\033[0m"
# define ft_RED			"\033[1;31m"
# define ft_GREEN		"\033[1;32m"
# define ft_YELLOW		"\033[1;33m"
# define WHITESPACE		" \n\r\t\f\v"
# include <iostream>
# include <vector>
# include <cstdlib>
# include <cstring>
# include <sstream>
# include <string>
# include <cstdio>		// std::remove
# include <dirent.h>
# include <sys/stat.h>
# include <signal.h>
# include <unistd.h>
# include <fstream>

// Enum used by Request.hpp && Response.hpp // SHOULD HAVE BEEN INSIDE THE http namespace BUT NOT ALL FILES CURRENTLY SUPPORTS THE namespace
enum ErrorCode
{
	NONE,
	OK = 200,
	MOVEDPERMANENTLY = 301,
	FOUND = 302,
	BADREQUEST = 400,
	UNAUTHORIZED = 401,
	FORBIDDEN = 403,
	NOTFOUND = 404,
	METHODNOTALLOWED = 405,
	REQUESTTIMEOUT= 408,
	CONTENTTOOLARGE = 413,
	URITOOLONG = 414,
	LISTDIRECTORYCONTENTS = 418,
	INTERNALSERVERERROR = 500,
	NOTIMPLEMENTED = 501,
	HTTPVERSIONNOTSUPPORTED = 505
};

namespace http {

	// **************** FUNCTIONS FOR PRINTING STATUS BEGINS ******************
	void	exit_with_error(const std::string& msg);
	void	print_status(const char *color, const std::string& msg);
	std::string getCurrentTime();
	std::string trim_whitespace(std::string str);
	std::string to_lower_case(std::string str);
	std::string to_upper_case(std::string str);
	size_t str_to_hex(std::string str);
	std::string remove_extra_backslash(std::string str);
	// **************** FUNCTIONS FOR PRINTING STATUS ENDS ********************
	///////////////////////////////////////////////////////////////////////////
	// **************** MISCELLANEOUS FUNCTIONS BEGINS 		*******************

	namespace ft {

		bool		isDirectory(const std::string &path);
		void		deleteDirectory(const std::string& loc_file_pat);
		void		listDirectoryContent(std::string& web_page, std::string loc_file_path, const std::string& root);
		std::string	translateErrorCode( const ErrorCode& status_code );
		void		initSignal( void );
		void		sig_handler(int sig, siginfo_t *info, void *context);

	}	// namespace ft

}	// namespace http

#endif
