# include "../includes/ConfigParser.hpp"

namespace http {

	ConfigParser::ConfigParser( void ) {}

	ConfigParser::ConfigParser(std::vector<Server>& servers, const char *path) {
		parse_config(servers, path);
	}

	ConfigParser::~ConfigParser( void ) {}

	ConfigParser::ConfigParser( const ConfigParser& other ) {
		(void) other;
	}

	ConfigParser& ConfigParser::operator= ( const ConfigParser& other ) {
		(void) other;
		return *this;
	}

	// ****************	CONSTRUCTORS && OPERATORS ENDS	**********************
	//////////////////////////////////////////////////////////////////////////
	// ******************* 		METHODS BEGINS			**********************

	// ******************************************************************
	// Simply reads from path and initializes X number of Server object	*
	// into the servers vector.											*
	// X representing the number of server contexts obtained from		*
	// the filepath dereferenced by path								*
	// ******************************************************************
	void	ConfigParser::parse_config(std::vector<Server>& servers, const char *path) {
		std::ifstream		fin;
		std::string			line;
		std::ostringstream	tmp;

		fin.open(path);

		if ( ! fin.good() )
			exit_with_error("Failed To Read From User-Submitted Config Filepath");

		print_status(ft_GREEN, "Config File Opened Successfully!");
		print_status(ft_YELLOW, "Now Parsing Config File...");

		while ( fin ) {
			std::getline(fin, line);
			erase_comments(line);
			if ( fin && line.size() > 0 )
				tmp << line << "\n";
			if ( fin && find_server_context(servers, tmp.str()) )
				tmp.str("");
		}
		if ( servers.size() == 0 )
			exit_with_error("Unable To Find Any valid server context in config file");

		print_status(ft_GREEN, "Config File Parsed Successfully!");
	}

	// ******************************************************
	// Verify that context contains a whole server context.	*
	// Then calls parse_one_server() to setup a Server		*
	// object using this server context.					*
	// Returns true if a Server object was configured.		*
	// Else it returns false								*
	// ******************************************************
	bool	ConfigParser::find_server_context(std::vector<Server>& servers,
													const std::string& context) {
		std::size_t		pos = 0;
		std::size_t		count = std::strlen("server");

		if ( (pos = context.find("server")) != std::string::npos && (pos == 0) )
			if ( (count = context.find_first_not_of("\t ", pos + count)) != std::string::npos )
				if ( ! context.compare(count, 1, "{", 0, 1) )
					if ( (count = calculate_closing_brace_pos(context.data() + pos)) ) 
						return parse_one_server(servers, context.substr(pos, count + 1));
		return false;
	}

	// ******************************************************************
	// Parses 1 server context, then save to servers(param1) using		*
	// servers.push_back()- The return value is true if successful, 	*
	// else it exits_with_error() at some point if error occured		*
	// ******************************************************************
	bool	ConfigParser::parse_one_server(std::vector<Server>& servers, std::string context) {
		Server		serv;
		std::string	max_body_tmp;

		print_status(ft_YELLOW, "Parsing Server Context...");

		while ( parse_one_location(serv.refLocations(), context) )
			;

		parse_to_str(serv.readIp(), "\nlisten", context);
		parse_to_str(serv.readName(), "\nserver_name", context);
		parse_to_str(serv.readRoot(), "\nroot", context);
		parse_to_str(serv.readErrorPage(), "\nerror_page", context);
		parse_to_str(max_body_tmp, "\nclient_max_body_size", context);
		parse_for_cgi(serv.getCgi(), context);

		setup_server_host(serv.refSockaddrs(), serv.readSockAddrLen(), serv.readIp(), serv.readPort());

		serv.writeMaxBody( ((std::atol(max_body_tmp.c_str()) > 0) ? std::atol(max_body_tmp.c_str()) : 0) );
		servers.push_back(serv);
		print_status(ft_GREEN, "Server Context Parsed Successfully!");

		return true;
	}

	// **********************************************************************
	// Uses the host address (IP:PORT) stored in ip(param3)					*
	// to setup serv._sockAddrs object(param1) and also splitting IP:PORT	*
	// stored in ip(param3) into separate IP and Port, then stored			*
	// in serv._ip and serv._port.											*
	// **********************************************************************
	void	ConfigParser::setup_server_host(struct sockaddr_in& _sockAddrs, const unsigned int& sockAddrs_len,
						const std::string& ip, const int& port) {
		unsigned int&	_sockAddrs_len(const_cast<unsigned int&>(sockAddrs_len));
		std::string&	_ip(const_cast<std::string&>(ip));
		int&			_port(const_cast<int&>(port));
		std::size_t		pos;

		if ( (pos = _ip.find(":")) != std::string::npos ) {
			if ( _ip.find_first_not_of("0123456789", ++pos) == std::string::npos ) {
				if ( (_port = std::atoi(_ip.substr(pos, std::strlen(_ip.data() + pos)).data())) >= 0
						&& _port <= MAX_16_BIT ) {
					_ip.erase(--pos);
					if ( _ip.size() >= MIN_HOST_LEN ) {
						_sockAddrs.sin_family = AF_INET;
						_sockAddrs.sin_port = htons(_port);
						_sockAddrs.sin_addr.s_addr = inet_addr(_ip.data());
						_sockAddrs_len = sizeof(_sockAddrs);
						if ( _sockAddrs.sin_addr.s_addr == static_cast<uint32_t>(-1) ) {
							std::ostringstream	msg;
							msg << "Cannot Interpret " << _ip << " As a Valid Local IP";
							exit_with_error(msg.str());
						}
					}
					else
						exit_with_error("Invalid Local IP provided");
				}
				else
					exit_with_error("Invalid 16-bit Host Port Number Provided");
			}
			else
				exit_with_error("Provided Host Port Contains Non-Numeric Values");	;
		}
	}

	// ******************************************************************
	// Parses 1 location context, then save to locations(param1) using	*
	// locations.push_back()- The return value is true if successful, 	*
	// else it returns false if no Location context was parsed.			*
	// ******************************************************************
	bool	ConfigParser::parse_one_location(std::vector<Location>& locations, std::string& context) {
		Location		loc;
		std::size_t		pos = 0;
		std::size_t		count = std::strlen("location");
		std::string		max_body_tmp;

		if ( (pos = context.find("location")) != std::string::npos && (context[pos - 1] == '\n') ) {
			if ( (count = context.find_first_not_of("\t ", pos + count)) != std::string::npos ) {
				if ( ! context.compare(count, 1, "/", 0, 1) ) {
					loc.writePath(context.substr(count, context.find_first_of("\t {", count +1) - count));

					int temp_begin = context.find_first_of("{", count +1) + 1;
					int temp_end = context.find_first_of("}", temp_begin);
					std::string _context = context.substr(temp_begin, temp_end - temp_begin);

					print_status(ft_YELLOW, "Parsing Location Context nested within Server...");

					parse_to_vect(loc.refMethods(), _context);
					parse_to_str(loc.readRoot(), "\nroot", _context);
					parse_to_str(loc.readIndex(), "\nindex", _context);
					parse_to_str(loc.readAutoind(), "\nautoindex", _context);
					parse_to_str(loc.readExec(), "\nexec", _context);
					parse_to_str(loc.readRewrite(), "\nrewrite", _context);
					parse_to_str(loc.readUploads(), "\nuploads", _context);
					parse_to_str(max_body_tmp, "\nclient_max_body_size", _context);

					loc.writeMaxBody( ((std::atol(max_body_tmp.c_str()) > 0) ? std::atol(max_body_tmp.c_str()) : 0) );
					locations.push_back(loc);
					context.erase(pos, temp_end - (pos - 1));

					print_status(ft_GREEN, "Location Context Parsed Successfully!");
					return true;
				}
				else
					exit_with_error("File/Dir path for a \"Location\" Context not set");
			}
		}
		return false;
	}

	// **************************************************************
	// Parses the method directive to Server::Location::_methods	*
	// vector of strings.											*
	// will have to be modified to become dynamic, in case we find	*
	// out that there is another simple directive which requires	*
	// its own vector of strings.									*
	// **************************************************************
	void	ConfigParser::parse_to_vect(std::vector<std::string>& methods, const std::string& context) {
		std::size_t		pos = 0;
		std::size_t		count = std::strlen("methods");

		if ( (pos = context.find("\nmethods")) != std::string::npos && (context[pos++] == '\n') ) {
			if ( (count = context.find_first_not_of("\t ", pos + count)) != std::string::npos ) {
				for (long unsigned int i = 0; ; ++i) {
					methods.push_back(context.substr(count, context.find_first_of(",;", count) - count));

					if ( !(methods.size() > i && methods[i].size() > 0 ) )
						break ;
					else if ( methods[i].find_first_of("\t ", 0) != std::string::npos )
						exit_with_error("methods Directive Not Properly Defined");
					
					count = context.find_first_of(",;", count);
					if ( context[count++] == ';')
						break ;
					count = context.find_first_not_of("\t ", count);
				}
			}	
		}
	}

	// **************************************************************
	// Parse a directive into a string in Server/Server::Location.	*
	// str(param1) is a reference to the string address to write.	*
	// name(param2) is the simple directive identifier.				*
	// context(param3) contains the current context to search		*
	// **************************************************************
	void	ConfigParser::parse_to_str(const std::string& str, const char *name, const std::string& context) {
		std::size_t			pos = 0;
		std::size_t			count = std::strlen(name + 1);
		std::string&		_str = const_cast<std::string&>(str);
		std::ostringstream	msg;

		if ( (pos = context.find(name)) != std::string::npos && context[pos++] == '\n' ) {
			if ( (count = context.find_first_not_of("\t ", pos + count)) != std::string::npos ) {
				_str = context.substr(count, context.find_first_of(";", count) - count);
				if ( _str.size() == 0 
					|| _str.find_first_of("\t ", 0) != std::string::npos ) {
						msg << name + 1 << " Directive Not Properly Defined";
						exit_with_error(msg.str());
					}
			}	
		}
		else if ( 	! std::strcmp("listen", name + 1) ||
					! std::strcmp("root", name + 1) ) { 			// for mandatory directives
			msg << "A Mandatory Simple Directive (" << name + 1
				<< ") is missing!";
			exit_with_error(msg.str());
		}
	
		// Add missing ending slash to these Directives if not yet present
		if ( ! std::strcmp("error_page", name + 1) ||
			 ! std::strcmp("uploads", name + 1) ||
			 ! std::strcmp("roots", name + 1) ) {
				 if ( (pos = _str.size()) && _str.at(pos - 1) != '/' )
				 	_str.push_back('/');
			 }
	}

	// ********************************************************************************
	// Calculates the number of hops from context[0] to position where closing '}'	  *
	// is found. It is guaranteed that context always has '{' before this function	  *
	// is called. Return value is number of hops. But if no matching closing '}'	  *
	// is found, it returns 0														  *
	// ********************************************************************************
	std::size_t	ConfigParser::calculate_closing_brace_pos(const std::string& context) {
		int	flag = 0;

		for (std::size_t i = 0; i < context.size(); ++i) {
			if (context[i] == '{')
				++flag;
			else if (context[i] == '}')
				if (--flag == 0)
					return (i);
		}
		return (0);
	}

	// ***************************************************
	// Erases (Space chars at line beginning) + Comments *
	// ***************************************************
	void	ConfigParser::erase_comments(std::string& line) {
		std::size_t				pos;

		if ( (line.size() > 0) &&
			(pos = line.find_first_not_of("\t ")) != std::string::npos )
			line.erase(line.begin(), line.begin() + pos);

		if ( (pos = line.find("#")) != std::string::npos )
			line.erase( line.begin() + pos, line.end() );
	}

	void ConfigParser::parse_for_cgi(std::map<std::string, std::pair<std::string, std::string> > &cgi_map, std::string &context)
	{
		size_t pos = context.find("\ncgi");
		size_t end_line = context.find(";", pos);
		size_t split_start;
		std::vector<std::string> tmp;
		while ( pos != std::string::npos)
		{
			split_start = context.find_first_of("\t", pos);
			tmp = split_string(context.substr(split_start + 1, end_line - 1 - split_start));
			if (tmp.size() != 3 || tmp[0][0] != '.' || !(tmp[1] == "GET" || tmp[1] == "POST"))
			{
				exit_with_error("Wrong format on cgi, need: extension, Method, exe name");
			}
			if (cgi_map.find(tmp[0])!= cgi_map.end())
				exit_with_error("Redeclaration of an extension for cgi");
			cgi_map[tmp[0]] = std::make_pair(tmp[1], tmp[2]);
			pos = context.find("\ncgi", end_line);
			end_line = context.find(";", pos);
		}
	}

	std::vector<std::string> ConfigParser::split_string(std::string str)
	{
		std::vector<std::string> result;
		size_t pos = str.find("\t");
		while (pos != std::string::npos)
		{
			result.push_back(str.substr(0, pos));
			str.erase(str.begin(), str.begin() + pos + 1);
			pos = str.find("\t");
		}
		result.push_back(str.substr(0, pos));
		return result;
	}

}	// namespace http
