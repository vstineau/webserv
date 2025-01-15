
//#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes/webserv.hpp"
#include <string>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/types.h>
#include <arpa/inet.h>


enum method
{
	GET,
	POST,
	DELETE//,
	//HEAD
};


struct config {
	std::string								server_name;
	std::string								host;
	std::string								http_redirection;
	std::string								directory_path;
	std::vector<method>	http_methods_allowed;;
	std::vector<int>	ports;
	int												client_body_size;
	bool											directory_listing;
	int												len_address;
};

struct request {
	std::string url;
	method method;
	std::map<std::string, std::string> headers;
	std::string body;
};

struct response {
	std::string version;
	int	status_code;
	std::string message;
	std::map<std::string, std::string> headers;
	std::string body;
};


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
	bool										_default_conf;
	struct sockaddr_in			_address;
};

#endif
