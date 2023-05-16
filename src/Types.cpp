#include "../includes/Types.hpp"

namespace http {

	namespace types {

		std::string	parseMime(const std::string& file_path)
		{
			Types			obj;
			std::size_t		pos;
			std::string		extension;

			if ( (pos = file_path.rfind('.', file_path.size() - 2)) != std::string::npos )
				extension = file_path.substr(pos);

			if ( extension.size() && obj.readMimeTypes().count(extension) )
				return (obj.readMimeTypes().at(extension));
			return (obj.readMimeTypes().at("default"));
		}

		bool	methodIsSupported(const std::string& method)
		{
			Types						obj;
			std::set<std::string>::iterator	it;

			if ( (it = obj.readMethodTypes().find(method)) != obj.readMethodTypes().end() )
				return (true);
			return false;
		}
	
	}	// namespace types

	// ***********************************************************************
	//////////////////////////////////////////////////////////////////////////
	// ***************	CONSTRUCTORS && OPERATORS BEGIN	++********************
	Types::~Types() {}

	Types::Types()
	{
		_mime_types[".html"] = "text/html";
		_mime_types[".htm"] = "text/html";
		_mime_types[".css"] = "text/css";
		_mime_types[".ico"] = "image/x-icon";
		_mime_types[".avi"] = "video/x-msvideo";
		_mime_types[".bmp"] = "image/bmp";
		_mime_types[".doc"] = "application/msword";
		_mime_types[".gif"] = "image/gif";
		_mime_types[".gz"] = "application/x-gzip";
		_mime_types[".ico"] = "image/x-icon";
		_mime_types[".jpg"] = "image/jpeg";
		_mime_types[".jpeg"] = "image/jpeg";
		_mime_types[".png"] = "image/png";
		_mime_types[".txt"] = "text/plain";
		_mime_types[".mp3"] = "audio/mp3";
		_mime_types[".pdf"] = "application/pdf";
		_mime_types["default"] = "text/html";

		_method_types.insert("GET");
		_method_types.insert("POST");
		_method_types.insert("DELETE");
		_method_types.insert("HEAD");
		_method_types.insert("PUT");
	}

	Types::Types( const Types& other ) : 	_mime_types(other._mime_types),
											_method_types(other._method_types) {
	}

	Types&	Types::operator= ( const Types& other ) {
		if (this != &other) {
			_mime_types = other._mime_types;
			_method_types = other._method_types;
		}
		return *this;
	}

	// ****************	CONSTRUCTORS && OPERATORS ENDS	**********************
	//////////////////////////////////////////////////////////////////////////
	// ******************* READING AND WRITING BEGINS	**********************
	const	std::map<std::string, std::string>&	Types::readMimeTypes( void ) { return _mime_types; }
	const	std::set<std::string>& 		Types::readMethodTypes( void ) { return _method_types; }

}	// namespace http
