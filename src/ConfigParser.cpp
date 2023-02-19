# include "../inc/ConfigParser.hpp"

ConfigParser::ConfigParser( void ) : _nb_server(0) {}

ConfigParser::~ConfigParser( void ) {}

ConfigParser::ConfigParser( const ConfigParser& other ) {
	_nb_server = other._nb_server;
}

ConfigParser& ConfigParser::operator= ( const ConfigParser& other ) {
	_nb_server = other._nb_server;
	return *this;
}
