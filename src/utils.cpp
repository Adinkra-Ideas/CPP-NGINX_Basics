# include "../includes/utils.hpp"

namespace ft {

	// **************** FUNCTIONS FOR PRINTING STATUS BEGINS ******************
	void	exit_with_error(const std::string& msg) {
		std::cout << ft_RED << "Error! " << msg << "!"
					<< ft_RESET_COLOR << std::endl;
		std::exit(1);
	}

	void	print_status(const char *color, const std::string& msg) {
		std::cout << color << "\n---------- " << msg
				<< " ----------" << ft_RESET_COLOR << std::endl;
	}

	// **************** FUNCTIONS FOR PRINTING STATUS ENDS ********************

}	// namespace ft