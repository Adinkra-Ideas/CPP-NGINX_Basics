#include <dirent.h> 
#include <stdio.h> 
# include <sstream>
# include <iostream>
# include <string>

// ******************************************************************************
// This function reads a directory's content from terminal, then builds it into	*
// a webpage displayable in a browser.											*
// wweb_page param1 is a ref to a string where our webpage will be stored.		*
// loc_file_path param2 is a copy of the local file path whose content we will 	*
// be using to build our webpage												*
// ******************************************************************************
void	listDirectoryContent(std::string& web_page, std::string loc_file_path, const std::string& root) {
			DIR					*d;
			struct dirent		*dir;
			std::ostringstream	tmp;

			std::size_t	pos = loc_file_path.size();
			if ( pos && loc_file_path.at(pos - 1) != '/')
				loc_file_path.push_back('/');

			d = opendir(loc_file_path.c_str());
			if (d) {
				loc_file_path.erase(0, root.size());
				std::cout << "\n\n\ntesting new path in listcontent " << loc_file_path << std::endl;
				tmp << "<html><head><title> test </title></head><body>";
				while ((dir = readdir(d)) != NULL) {
					tmp << "<p><a href=\"" << loc_file_path << dir->d_name << "\">" << dir->d_name << "</a></p>";
				}
				tmp << "</body></html>";
				web_page = tmp.str();
				closedir(d);
			}
		}

int main(void) {
	std::string web;
	std::string loc("public_html");
	std::string root("public");

	listDirectoryContent(web, loc, root);
	std::cout << web << std::endl;

	return(0);
}

