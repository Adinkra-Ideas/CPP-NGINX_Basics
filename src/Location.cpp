# include "../includes/Location.hpp"

namespace http {

	Location::Location( void ) {}

	Location::~Location( void ) {}

	Location::Location( const Location& other ) : _path(other._path), _methods(other._methods),
					_root(other._root), _index(other._index), _autoindex(other._autoindex),
					_exec(other._exec) {
		// std::cout << "Location copy constructor called " << std::endl;
		(void) other;
	}

	Location& Location::operator= ( const Location& other ) {
		if (this != &other)
		{
			_path = other._path;
			_methods = other._methods;
			_root = other._root;
			_index = other._index;
			_autoindex = other._autoindex;
			_exec = other._exec;
		}
		return *this;
	}

	// ****************	CONSTRUCTORS && OPERATORS ENDS	**********************
	//////////////////////////////////////////////////////////////////////////
	// ******************* 		METHODS BEGINS			**********************
	std::vector<std::string>& Location::refMethods( void ) { return _methods; }
		
	void	Location::writePath(const std::string& path) { _path = path; }
	const std::string&	Location::readPath( void ) { return _path; }

	void	Location::writeRoot(const std::string& root) { _root = root; }
	const std::string&	Location::readRoot( void ) { return _root; }

	void	Location::writeIndex(const std::string& index) { _index = index; }
	const std::string&	Location::readIndex( void ) { return _index; }

	void	Location::writeAutoind(const std::string& autoind) { _autoindex = autoind; }
	const std::string&	Location::readAutoind( void ) { return _autoindex; }

	void	Location::writeExec(const std::string& exec) { _exec = exec; }
	const std::string&	Location::readExec( void ) { return _exec; }

}	// namespace ft
