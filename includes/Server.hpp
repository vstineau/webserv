
#pragma once
#include "../includes/webserv.hpp"

class Server {

public:
	Server();
	Server(config conf_file);
	~Server();

	int			SetServFd(void);
	void		SetRequest(int fd, request &request);
	int			bindListenAccept(void);

private:
	config									_conf;
	std::map<int, request>	_requests;
	int											_server_fd;
	int											_client_fd;
	int											_backlog;
	int											_domain;
	int											_service;
	int											_protocol;
};
