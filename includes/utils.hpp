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

namespace ft {

	// **************** FUNCTIONS FOR PRINTING STATUS BEGINS ******************
	void	exit_with_error(const std::string& msg);
	void	print_status(const char *color, const std::string& msg);

	// **************** FUNCTIONS FOR PRINTING STATUS ENDS ********************

}	// namespace ft

#endif
