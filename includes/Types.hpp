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
		typedef	typename std::map<std::string, std::string> str_to_str_map;
		typedef typename std::set<std::string>				str_set;

		// ****** Constructors and Destructor **********
		~Types();
		Types();
		Types( const Types& other );

		// ************* Operators *********************
		Types& operator= ( const Types& other );

		// ************* Reading & Writing Operations **********
		const	str_to_str_map& readMimeTypes( void );
		const	str_set& 		readMethodTypes( void );

	private:
		str_to_str_map	_mime_types;
		str_set			_method_types;
	};

}	// namespace http

#endif
