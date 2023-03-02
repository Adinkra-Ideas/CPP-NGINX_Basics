#include "ServerConfig.hpp"
#include <cstring>
#include <cerrno>
ServerConfig::ServerConfig( void ) : _listen_fd(), _server_address(),
					_server_address_len(sizeof(_server_address))
{

}

ServerConfig::~ServerConfig( void ) {
}

ServerConfig::ServerConfig( const ServerConfig& other ) {
	_listen_fd = other._listen_fd;
	this->_host = other._host;
	this->_port = other._port;
	this->_server_address = other._server_address;
	this->_server_address_len = other._server_address_len;
}

ServerConfig& ServerConfig::operator= ( const ServerConfig& other ) {
	this->_listen_fd = other._listen_fd;
	this->_host = other._host;
	this->_port = other._port;
	this->_server_address = other._server_address;
	this->_server_address_len = other._server_address_len;
	return *this;
}

int&	ServerConfig::getListen_fd( void ) { return _listen_fd; }

sockaddr_in&	ServerConfig::getServer_address( void ) { return _server_address; }

unsigned int&	ServerConfig::getServer_address_len( void ) { return _server_address_len; }

void ServerConfig::setHost(std::string host)
{
	this->_host = inet_addr(host.data());
}

void ServerConfig::setPort(std::string port)
{
	unsigned int tmp_port;
	std::cout << port << std::endl;
	tmp_port = 8080;
	this->_port = (uint16_t) tmp_port;
}
void	ServerConfig::setupServer()
{
	if ( (_listen_fd = socket(AF_INET, SOCK_STREAM, 0) )  == -1 ) {
		//log error msg
		
		exit(1);
	}
	memset(&_server_address, 0, sizeof(_server_address));
	_server_address.sin_family = AF_INET;
	_server_address.sin_port = htons(this->_port);
	_server_address.sin_addr.s_addr = this->_host;

	if (bind(_listen_fd, (struct sockaddr *)&_server_address, _server_address_len) == -1)
    {
		std::cout << strerror(errno) <<std::endl;

        exit(1);
    }

	// Check data stored in Socket Address
	std::cout << "\nSocket FD " << _listen_fd <<
		" bounded successfully with Socket Address "
		<< inet_ntoa(_server_address.sin_addr) << ":" <<
		ntohs(_server_address.sin_port) << std::endl;
}
