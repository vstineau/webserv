
#pragma once
#include "../includes/webserv.hpp"

class Server {

public:
	Server();
	Server(config conf_file);
	~Server();

	int			SetServFd(void);
	void		SetRequest(void);
	void		SetResponse(void);
	int			bindListenAccept(void);

private:
	void	_fillRequest(request &request, std::string &buffer);
	void	_methodGET(request &request, std::string &buffer);
	void	_methodPOST(request &request, std::string &buffer);
	void	_methodDELETE(request &request, std::string &buffer);
	config									_conf;
	std::map<int, request>	_requests;
	response								_response;
	int											_server_fd;
	int											_client_fd;
	int											_backlog;
	int											_domain;
	int											_service;
	int											_protocol;
};
