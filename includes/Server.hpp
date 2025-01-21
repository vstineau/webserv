
#pragma once
//#ifndef SERVER_HPP
//#define SERVER_HPP

#include "../includes/webserv.hpp"
#include <string>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/types.h>
#include <arpa/inet.h>


class Server {

public:
	Server();
	Server(config conf_file);
	~Server();

	int			setServFd(void);
	void		setRequest(void);
	void		SetResponse(void);
	int			bindListenAccept(void);
	void		print_response(response &rep);
	void		print_request(request &rep);
	int			getServer_fd() const { return _server_fd;}
	int			getclient_fd() const { return _client_fd;}

private:
	void	_fillRequest(request &request, std::string &buffer);
	std::string	_responseGET(std::string buffer);
	std::string	_responsePOST(std::string buffer);
	std::string	_responseDELETE(std::string buffer);
	config									_conf;
	std::map<int, request>	_requests;
	response								_response;
	int											_server_fd;
	int											_client_fd;
	bool										_default_conf;
	struct sockaddr_in			_address;
};

//#endif
