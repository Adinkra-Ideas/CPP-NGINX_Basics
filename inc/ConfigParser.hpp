#pragma once
#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>

class ConfigParser {
public:
	ConfigParser( void );
	~ConfigParser( void );
	ConfigParser( const ConfigParser& other );

	ConfigParser& operator= ( const ConfigParser& other );


private:
	unsigned int	_nb_server;

};

#endif
