# include "../includes/utils.hpp"

namespace http {

	// **************** FUNCTIONS FOR PRINTING STATUS BEGINS ******************
	void	exit_with_error(const std::string& msg) {
		std::cerr << ft_RED << "Error! " << msg << "!"
					<< ft_RESET_COLOR << std::endl;
		std::exit(1);
	}

	void	print_status(const char *color, const std::string& msg) {
		std::cout << color << getCurrentTime() << msg << ft_RESET_COLOR << std::endl;
	}

	std::string getCurrentTime()
	{
		time_t rawtime;
  		struct tm * timeinfo;
  		char buffer [80];

  		time(&rawtime);
  		timeinfo = localtime(&rawtime);
		strftime (buffer,80,"[%Y-%m-%d  %H:%M:%S]   ",timeinfo);
		return (std::string(buffer));
	}
	// **************** FUNCTIONS FOR PRINTING STATUS ENDS ********************

}	// namespace ft