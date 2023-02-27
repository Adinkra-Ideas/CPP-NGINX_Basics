# include "ConfigParser.hpp"

ConfigParser::ConfigParser( void ) : servers(), _nb_server(0) {}

ConfigParser::~ConfigParser( void ) {}

ConfigParser::ConfigParser( const ConfigParser& other ) {
	_nb_server = other._nb_server;
	this->servers = other.servers;
}

ConfigParser& ConfigParser::operator= ( const ConfigParser& other ) {
	_nb_server = other._nb_server;
	this->servers = other.servers;
	return *this;
}

//functions to create the cluster of servers
//default just for testing atm, other takes the config file as imput
void ConfigParser::createCluster()
{
	ServerConfig server;
	std::string tmp = "Hello world";
	createServer(tmp, server);
	this->servers.push_back(server);
}

void ConfigParser::createCluster(std::string configFile)
{
	std::cout << configFile << std::endl;
}

std::vector<ServerConfig> ConfigParser::getServers()
{
	return (this->servers);
}

//creates a singel server from the config file or parts?
void ConfigParser::createServer(std::string &config, ServerConfig &server)
{
	std::cout << config << std::endl;
	server.setHost("127.0.0.1");
	server.setPort("8080");
}
