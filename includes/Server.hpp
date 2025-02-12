
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

//%{code error}%
static std::string error_body = "<!DOCTYPE html>" ENDL
"<html lang=\"en\"> " ENDL
"<head>" ENDL
"	<meta charset=\"UTF-8\">" ENDL
"	<meta name=\"error\" content=\"width=device-width, initial-scale=1.0\">" ENDL
"		<title>%{code error}%</title>" ENDL
"		<style> body{	background: content-box radial-gradient(#0664dc, #33b203);" ENDL
"									font-family: cursive;" ENDL
"									font-size: 3.2rem;" ENDL
"									display: flex;" ENDL
"									width: 100%;" ENDL
"								height: 100%;" ENDL
"								justify-content: center;" ENDL
"								}" ENDL
"		</style>" ENDL
"</head>" ENDL
"<body>" ENDL
"		<img src=https://http.cat/400.jpg /> " ENDL
"</body>" ENDL; 


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
