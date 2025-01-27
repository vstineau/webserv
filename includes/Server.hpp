
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
	Server(config &conf_file);
	~Server();

	int			setServFd(void);
	void		SetResponse(void);
	int			setServer_fd(void);
	void		print_response(response &rep);
	void		print_request(int n);
	void		fillRequest(int n, std::string &buffer);
	//int			getServer_fd() const { return server_fd;}
	//int			getClient_fd() const { return _client_fd;}
	//void		setClient_fd(int fd) { _client_fd = fd;}
	struct sockaddr_in			getAddress() const { return address;}
	int											server_fd;
	int											client_fd;
	struct sockaddr_in			address;

private:
	std::string	_responseGET(int n);
	std::string	_responsePOST(int n);
	std::string	_responseDELETE(int n);
	config									_conf;
	std::map<int, request>	_requests;
	response								_response;
};

//#endif
