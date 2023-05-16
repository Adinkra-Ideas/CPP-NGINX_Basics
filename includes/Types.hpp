#pragma once
#ifndef __MIME_HPP__
#define __MIME_HPP__

# include <iostream>
# include <string>
# include <map>
# include <set>

namespace http {

	namespace types {

		std::string	parseMime(const std::string& extension);
		bool		methodIsSupported(const std::string& method);

	}	// namespace types

	class Types
	{
	public:
		// ****** Constructors and Destructor **********
		~Types();
		Types();
		Types( const Types& other );

		// ************* Operators *********************
		Types& operator= ( const Types& other );

		// ************* Reading & Writing Operations **********
		const	std::map<std::string, std::string>& readMimeTypes( void );
		const	std::set<std::string>& 		readMethodTypes( void );

	private:
		std::map<std::string, std::string>	_mime_types;
		std::set<std::string>			_method_types;
	};

}	// namespace http

#endif
