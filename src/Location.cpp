# include "../includes/Location.hpp"

namespace ft {

		Location::Location( void ) {}

		Location::~Location( void ) {}

		Location::Location( const Location& other ) {
			(void) other;
		}

		Location& Location::operator= ( const Location& other ) {
			(void) other;
			return *this;
		}

}	// namespace ft
