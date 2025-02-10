
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
	Server(config &conf);
	~Server();

	int			setServFd(void);
	void		SetResponse(int n);
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
	void				fill_header(std::string &header, int &n);
	void				fill_body(std::string &header, int &n);
	void				create_img(std::string &img);
	std::size_t	check_contentype(int n, std::size_t pos, std::size_t offset, std::string &buffer);
	void				_responseGET(request &rep);
	void				_responsePOST(request &rep);
	void				_responseDELETE(request &rep);
	config									_conf;
	std::map<int, request>	_requests;
	response								_response;
};

//#endif
