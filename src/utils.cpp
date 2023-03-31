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

		void	deleteDirectory(std::string& loc_file_path) {

			if (loc_file_path.at(loc_file_path.size() - 1) != '/')
				loc_file_path.push_back('/');

			struct dirent	*dir;
			DIR 			*d = opendir(loc_file_path.c_str());
			if ( d ) {
				while ( (dir = readdir(d)) != NULL ) {
					std::remove( (loc_file_path + dir->d_name).c_str() );
				}
				std::remove( loc_file_path.c_str() );
				closedir(d);
			}
		}

		// ******************************************************************************
		// This function reads a directory's content from terminal, then builds it into	*
		// a browser-displayable-webpage format 										*
		// web_page param1 is a reference to a string where our webpage will be stored.	*
		// loc_file_path param2 is a copy of the local file path whose content we will 	*
		// be using to build our webpage												*
		// root param3 is the name of directory used as root, which had to be concate-	*
		// nated with httprequest path to form loc_file_path							*
		// ******************************************************************************
		void	listDirectoryContent(std::string& web_page, std::string loc_file_path, const std::string& root) {
			DIR					*d;
			struct dirent		*dir;
			std::ostringstream	tmp;

			std::size_t	pos = loc_file_path.size();
			if ( pos && loc_file_path.at(pos - 1) != '/')
				loc_file_path.push_back('/');

			d = opendir(loc_file_path.c_str());
			if ( d ) {
				loc_file_path.erase(0, root.size());
				tmp << "<html><head><title> Directory </title></head><body>";
				while ( (dir = readdir(d)) != NULL ) {
					tmp << "<p><a href=\"" << loc_file_path << dir->d_name << "\">" << dir->d_name << "</a></p>";
				}
				tmp << "</body></html>";
				web_page = tmp.str();
				closedir(d);
			}
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
				case 418:
					return "LISTDIRECTORYCONTENTS";
				case 505:
					return "HTTP VERSION NOT SUPPORTED";
				default:
					return "UNKNOWN RESPONSE";	// Optional
			}
		}

	}	// namespace ft

}	// namespace http
