# include "../includes/ConfigParser.hpp"

namespace ft {

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
	// else it returns false if no Location context was parsed.			*
	// ******************************************************************
	bool	ConfigParser::parse_one_server(std::vector<Server>& servers, std::string context) {
		Server	serv;

		print_status(ft_YELLOW, "Parsing Server Context...");

		while ( parse_one_location(serv.refLocations(), context) )
			;

		parse_to_str(serv.readListen(), "\nlisten", context);
		parse_to_str(serv.readName(), "\nserver_name", context);
		parse_to_str(serv.readRoot(), "\nroot", context);
		parse_to_str(serv.readMaxBody(), "\nclient_max_body_size", context);
		parse_to_str(serv.readErrorPage(), "\nerror_page", context);

		servers.push_back(serv);

		print_status(ft_GREEN, "Server Context Parsed Successfully!");

		return true;
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
		else if ( ! std::strcmp("index", name + 1) ||
					! std::strcmp("listen", name + 1) ||
					! std::strcmp("root", name + 1) ||
					! std::strcmp("error_page", name + 1) ) { // for mandatory directives
			msg << "A Mandatory Simple Directive (" << name + 1
				<< ") is missing!";
			exit_with_error(msg.str());
		}
	}

	// ********************************************************************************
	// Calculates the number of hops from context[0] to positio where closing '}'	  *
	//  is found. It is guaranteed that context always has '{' before this function	  *
	//  is called. Return value is number of hops. But if no matching closing '}'	  *
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

}	// namespace ft