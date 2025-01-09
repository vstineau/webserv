
#ifndef SERVER_HPP
# define SERVER_HPP

#include "../includes/webserv.hpp"

class Server {

public:
	Server();
	Server(config conf_file);
	Server(Server const & src);
	~Server();
	Server &operator=(Server const & src);

private:
	config									_conf;
	std::map<int, request>	_requests;
	int											_server_fd;
	int											_backlog;
	int											_domain;
	int											_service;
	int											_protocol;
};

#endif
