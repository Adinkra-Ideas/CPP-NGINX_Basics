#pragma once
#ifndef __UTILS_HPP__
# define __UTILS_HPP__

# define ft_RESET_COLOR	"\033[0m"
# define ft_RED			"\033[1;31m"
# define ft_GREEN		"\033[1;32m"
# define ft_YELLOW		"\033[1;33m"

# include <iostream>
# include <vector>
# include <cstdlib>
# include <sstream>
# include <string>

# include <sys/stat.h>

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
	CONTENTTOOLARGE = 413,
	NOTIMPLEMENTED = 501
};

namespace http {

	// **************** FUNCTIONS FOR PRINTING STATUS BEGINS ******************
	void	exit_with_error(const std::string& msg);
	void	print_status(const char *color, const std::string& msg);
	std::string getCurrentTime();

	// **************** FUNCTIONS FOR PRINTING STATUS ENDS ********************
	///////////////////////////////////////////////////////////////////////////
	// **************** MISCELLANEOUS FUNCTIONS BEGINS 		*******************

	namespace ft {

		bool		isDirectory(const std::string &path);
		std::string	translateErrorCode( const ErrorCode& status_code );

	}	// namespace ft

}	// namespace http

#endif
