#pragma once
#ifndef __CONFIGPARSER_HPP__
# define __CONFIGPARSER_HPP__

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
