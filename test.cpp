# include <iostream>
# include <string>

int main() 
{
	std::string request_path = " /go/.html";
	std::string request_dir;

	std::size_t		pos;

		// erase spaces at the beginning (if any)
		pos = request_path.find_first_not_of(' ');
		if ( pos )
			request_path.erase(0, pos);
		
		// put '/' at the beginning if absent
		if ( request_path.size() > 0 && request_path.at(0) != '/' )
			request_path.insert(0, 1, '/');

		// In the client's http request, split the directory from path. 
		// Store directory to dir_sign and filename to web_url_path
		// if ( request_path.size() > 0) {
		// pos = dir_sign.find_first_of('/');
		// 	web_url_path = dir_sign.substr(pos);
		// 	dir_sign.erase(pos + 1);
		pos = request_path.size();
		if ( pos > 2 && request_path.at(pos - 1) == '/' )	// >2 meaning at least std::strlen("/x/")
			request_path.erase(pos - 1, 1);	// we want to search Location context using z.B '/web_url_path' not '/web_url_path/'
		
		// extract directory path from path request_path
		request_dir = request_path;
		if ( (pos = request_dir.find('.')) != std::string::npos ) {
			if ( (pos = request_dir.rfind('/', pos)) != std::string::npos )
				request_dir = request_dir.substr(0, pos);
			if ( pos == std::string::npos || request_path.at(request_dir.size() + 1) == '.' ) // we dont want "www.google.com/."
				std::cout <<  "BADREQUEST" << std::endl;
		}

	std::cout <<"path |" << request_path << "| dir |" << request_dir << "|" << std::endl;

	return 0;
}
