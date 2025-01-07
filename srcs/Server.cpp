
#include "Server.hpp"

Server::Server()
{}

Server::Server(config conf): _conf(conf)
{}

Server::~Server()
{}

Server::Server(Server const & src)
{}

Server& Server::operator=(Server const & src)
{
	*this = src;
	return (*this);
}

