# include "../inc/ServerConfig.hpp"

ServerConfig::ServerConfig( void ) : _listen_fd(), _server_address(),
					_server_address_len(sizeof(_server_address)) {

}

ServerConfig::~ServerConfig( void ) {
}

ServerConfig::ServerConfig( const ServerConfig& other ) {
	_listen_fd = other._listen_fd;
}

ServerConfig& ServerConfig::operator= ( const ServerConfig& other ) {
	_listen_fd = other._listen_fd;
	return *this;
}

int&	ServerConfig::getListen_fd( void ) { return _listen_fd; }

sockaddr_in&	ServerConfig::getServer_address( void ) { return _server_address; }

unsigned int&	ServerConfig::getServer_address_len( void ) { return _server_address_len; }

void	ServerConfig::setupServer( std::pair<std::string, int>& sock_addr )
{
	if ( (_listen_fd = socket(AF_INET, SOCK_STREAM, 0) )  == -1 ) {
		//log error msg
		exit(1);
	}

	_server_address.sin_family = AF_INET;
	_server_address.sin_port = htons(sock_addr.second);
	_server_address.sin_addr.s_addr = inet_addr(sock_addr.first.data()) ;

	if (bind(_listen_fd, (sockaddr *)&_server_address, _server_address_len) == -1)
    {
		// Logger::logMsg(RED, CONSOLE_OUTPUT, "webserv: bind error %s   Closing ....", strerror(errno));
        exit(1);
    }

	// Check data stored in Socket Address
	std::cout << "\nSocket FD " << _listen_fd <<
		" bounded successfully with Socket Address "
		<< inet_ntoa(_server_address.sin_addr) << ":" <<
		ntohs(_server_address.sin_port) << std::endl;
}
