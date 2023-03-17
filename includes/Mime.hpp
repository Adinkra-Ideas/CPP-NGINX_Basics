#pragma once
#ifndef __MIME_HPP__
#define __MIME_HPP__

# include <iostream>
# include <string>
# include <map>

namespace http {

	namespace mime {

		std::string	getMimeType(const std::string& extension);

	}	// namespace mime

	class Mime
	{
	public:
		friend std::string	mime::getMimeType(const std::string& extension);

		Mime();

	private:
		std::map<std::string, std::string> _mime_types;     
	};

}	// namespace http

#endif
