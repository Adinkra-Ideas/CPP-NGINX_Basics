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

	namespace ft {

		// Only returns true when path is a directory
		bool	isDirectory(const std::string &path)
		{
			struct stat file_stat;

			if (stat(path.c_str(), &file_stat) != 0)
				return (false);

			return (S_ISDIR(file_stat.st_mode));
		}

		// ******************************************************************
		// Converts the error code stored in status_code into the readable	*
		// error message. status_code is a var of data type ErrorCode enum	*
		// ******************************************************************
		std::string	translateErrorCode( const ErrorCode& status_code ) {
			switch (status_code) {
				case 200:
					return "OK";
				case 301:
					return "MOVEDPERMANENTLY";
				case 302:
					return "FOUND";
				case 403:
					return "FORBIDDEN";
				case 404:
					return "NOT FOUND";
				case 405:
					return "METHOD NOT ALLOWED";
				case 413:
					return "CONTENT TOO LARGE";
				default:
					return "UNKNOWN RESPONSE";	// Optional
			}
		}

	}	// namespace ft

}	// namespace http