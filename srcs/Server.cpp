
#include "../includes/Server.hpp"
#include "../includes/color.hpp"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

Server::Server() :	server_fd(0),
										client_fd(0),
										address()
{
}

Server::Server(config &conf):	server_fd(0),
															client_fd(0),
															_conf(conf)
{}

Server::~Server()
{
	if (server_fd != -1)
		close(server_fd);
	if (client_fd != -1)
		close(client_fd);
}

std::string	Server::_responseGET(request &req)
{
	(void)req;
	std::string rep;
	return (rep);
}

std::string	Server::_responsePOST(request &req)
{
	(void)req;
	std::string rep;
	return (rep);
}

std::string	Server::_responseDELETE(request &req)
{
	(void)req;
	std::string rep;
	return (rep);
}

void	Server::SetResponse(request &req, std::string &rep)
{
	if (req.method == GET)
		rep = _responseGET(req);
	else if (req.method == GET)
		rep = _responsePOST(req);
	else if (req.method == GET)
		rep = _responseDELETE(req);
}

void Server::create_img(std::string &img)
{
	size_t pos = 0;
	size_t offset = 0;
	std::string filename;
	std::string content;
	
	if (chdir("www/data"))
		std::cout << "CHDIR FAILED\n";
	pos = img.find("filename=\"", offset);
	if (pos == std::string::npos){ return ;}
	offset = pos + 10;
	pos = img.find("\"", offset);
	filename = img.substr(offset, pos - offset);
	offset = pos + 1;
	std::ofstream ofs(filename.c_str(), std::ios_base::binary);
	if (!ofs)
		std::cout << "error oppenning new file \n";
	offset = pos + 1;
	pos = img.find("\r\n\r\n", offset);
	if (pos == std::string::npos){ return ;}
	offset = pos + 4;
	content = img.substr(offset, img.size() - offset);
	ofs << content;
	if (chdir("../.."))
		std::cout << "CHDIR FAILED\n";
}

void Server::fill_body(std::string &body, int &n)
{
	size_t pos = 0;
	size_t offset = 0;
	std::string img;
	while (pos != std::string::npos)
	{
		pos = body.find("\n", offset);
		if (pos == std::string::npos){return ;}
		offset = pos + 1;
		pos = body.find(_requests[n].headers["boundary"], offset);
		if (pos == std::string::npos){return ;}
		img = body.substr(offset, pos - offset);
		create_img(img);
		offset = pos + _requests[n].headers["boundary"].size();
	}
}

void Server::fill_header(std::string &header, int &n)
{
	size_t pos = 0;
	size_t offset = 0;
	std::string key;
	while (pos != std::string::npos)
	{
		pos = header.find(":", offset);
		if (pos == std::string::npos){ return ;}
		key = header.substr(offset, pos - offset);
		offset = pos + 1;
		if (key == "Content-Type")
		{
			pos = check_contentype(n, pos, offset, header);
			if (pos == std::string::npos){ break;}
		}
		else
		{
			pos = header.find("\n", offset);
			if (pos == std::string::npos){ break;}
			_requests[n].headers[key] = header.substr(offset, pos - offset);
		}
		offset = pos + 1;
		//std::cout << "aaaaaaaaa\n";
	}
}

std::size_t	Server::check_contentype(int n, std::size_t pos, std::size_t offset, std::string &buffer)
{
	std:: string tmp;
	std:: string key1("Content-Type");
	std:: string key2("boundary");
	std::size_t posendline;

	posendline = buffer.find("\r\n", offset);
	if (posendline == std::string::npos)
		return (posendline);
	tmp = buffer.substr(offset, pos - offset);
	pos = buffer.find(key2, offset);
	if (pos == std::string::npos)
	{
		_requests[n].headers[key1] = buffer.substr(offset, posendline - offset);
		return (posendline);
	}
	_requests[n].headers[key1] = buffer.substr(offset, pos - offset);
	offset = pos + 10;
	_requests[n].headers[key2] = buffer.substr(offset, posendline - offset);
	return (posendline);
}

void Server::fillRequest(int n, std::string &buffer)
{
	size_t pos = 0;
	size_t offset = 0;
	std::string header;
	std::string body;

	if (!buffer.find("GET"))
	{
		_requests[n].method = GET;
		offset = pos + 3;
	}
	else if (!buffer.find("POST"))
	{
		_requests[n].method = POST;
		offset = pos + 4;
	}
	else if (!buffer.find("DELETE"))
	{
		_requests[n].method = DELETE;
		offset = pos + 6;
	}
	pos = buffer.find("/", offset);
	if (pos == std::string::npos){ return;}
	offset = pos;
	pos = buffer.find(" ", offset);
	if (pos == std::string::npos){ return;}
	_requests[n].path = buffer.substr(offset, pos - offset);
	offset = pos + 1;
	pos = buffer.find("\n", offset);
	if (pos == std::string::npos){ return;}
	_requests[n].version = buffer.substr(offset, pos - offset);
	offset = pos + 1;
	pos = buffer.find("\r\n\r\n", offset);
	if (pos == std::string::npos){ return;}
	header = buffer.substr(offset, pos - offset);
	fill_header(header, n);
	offset = pos + 4;
	_requests[n].body = buffer.substr(offset, buffer.size() - offset);
	if (!_requests[n].headers["boundary"].empty())
		fill_body(_requests[n].body, n);
}

//link server socket to the IP adress and to the port(s), start listenning then accept connection from client
int Server::setServer_fd()
{
	struct sockaddr_in			addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;//inet_addr("0.0.0.0");
	addr.sin_port = htons(8080);
	address = addr;

	int b = true;
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		perror("socket");
	}
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &b, sizeof(int));
	if (bind(server_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
	{
		perror("bind");
		close(server_fd);
		return (1);
	}
	if (listen(server_fd, 10) == -1)
	{
		perror("listen");
		close(server_fd);
		return (1);
	}
	return (0);
}
