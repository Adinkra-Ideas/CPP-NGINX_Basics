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
	// X signifies the number of server contexts (NGINX) obtained from	*
	// the filepath path												*
	// ******************************************************************
	void	ConfigParser::parse_config(std::vector<Server>& servers, const char *path) {
		std::ifstream		fin;
		std::string			line;
		std::ostringstream	tmp;

		fin.open(path);

		if ( ! fin.good() )
			exit_with_error("Failed To Read From User-Submitted Config Filepath");
		else
			print_status(ft_YELLOW, "Config File Opened Successfully!");

		while ( fin ) {
			std::getline(fin, line);
			erase_comments(line);
			if ( fin && line.size() > 0 )
				tmp << line << "\n";
			if ( fin && verif_and_parse_one_server(servers, tmp.str()) )
				tmp.str("");
		}
	}

	// ******************************************************
	// Verify that block contains a whole server context.	*
	// Then calls configure_one_server() to setup a Server	*
	// object using this server context.					*
	// Returns true if a Server object was configured.		*
	// Else it returns false								*
	// ******************************************************
	bool	ConfigParser::verif_and_parse_one_server(std::vector<Server>& servers,
													const std::string& block) {
		std::size_t		pos = 0;
		std::size_t		count = std::strlen("server");

		if ( (pos = block.find("server")) != std::string::npos && (pos == 0) )
			if ( (count = block.find_first_not_of("\t ", pos + count)) != std::string::npos )
				if ( ! block.compare(count, 1, "{", 0, 1) )
					if ( (count = calculate_closing_brace_pos(block.data() + pos)) ) 
						if ( configure_one_server(servers, block.substr(pos, count + 1)) )
							return true;
		return false;
	}

	// We're here
	bool	ConfigParser::configure_one_server(std::vector<Server>& servers, std::string block) {
		// Server	tmp;
		(void) servers;

		// FOR TEST PURPOSE
		std::cout << block << std::endl;

		return true;
	}

	// ********************************************************************************
	// Calculates the number of hops from block[0] to pos where closing '}' is found. *
	// It is guaranteed that block always has '{' before this function is called.	  *
	//  <Return value is number of hops. But if no matching closing '}'				  *
	// is found, it returns 0														  *
	// ********************************************************************************
	std::size_t	ConfigParser::calculate_closing_brace_pos(const std::string& block) {
		int	flag = 0;

		for (std::size_t i = 0; i < block.size(); ++i) {
			if (block[i] == '{')
				++flag;
			else if (block[i] == '}')
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