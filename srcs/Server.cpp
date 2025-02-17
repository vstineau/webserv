
#include "../includes/Server.hpp"
#include "../includes/color.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

Server::Server() :	server_fd(-1),
										client_fd(-1),
										address()
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
	}
	if (listen(server_fd, 10) == -1)
	{
		perror("listen");
		close(server_fd);
	}
}

Server::Server(config &conf):	server_fd(-1),
															client_fd(-1),
															_conf(conf)
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
	}
	if (listen(server_fd, 10) == -1)
	{
		perror("listen");
		close(server_fd);
	}
}

Server::~Server()
{
	if (server_fd != -1)
		close(server_fd);
	if (client_fd != -1)
		close(client_fd);
}

void	Server::_responseGET(request &req)
{
	_response.status_line = "HTTP/1.1 ";
	(void)req;
	return ;
}

void	Server::_responsePOST(request &req)
{
	_response.status_line = "HTTP/1.1 ";
	(void)req;
	//gnegnegne POSTfailed
	//_response.status_code = "403 Forbidden";
	//gnegnegne POSTsuccessfull
	_response.status_line += "200 OK";
	return ;
}

void	Server::_responseDELETE(request &req)
{
	_response.status_line = "HTTP/1.1 ";
	(void)req;
	if (unlink(req.path.c_str()) == -1)
		SetResponseStatus(404);

	//gnegnegne deletesucessfull
	_response.status_line += "200 OK";
	return ;
}

void	Server::SetResponseStatus(int n)
{
	_response.body = get_body_error(n);
}

void	Server::SetResponse(int n)
{
	if (_requests[n].method == GET)
		 _responseGET(_requests[n]);
	else if (_requests[n].method == POST)
		 _responsePOST(_requests[n]);
	else if (_requests[n].method == DELETE)
		_responseDELETE(_requests[n]);
}

void Server::create_img(std::string &img)
{
	size_t pos = 0;
	size_t offset = 0;
	std::string filename;
	std::string content;
	
	if (chdir("www/data"))
		std::cerr << "CHDIR FAILED\n";
	pos = img.find("filename=\"", offset);
	if (pos == std::string::npos){ return ;}
	offset = pos + 10;
	pos = img.find("\"", offset);
	filename = img.substr(offset, pos - offset);
	offset = pos + 1;
	std::ofstream ofs(filename.c_str(), std::ios_base::binary);
	if (!ofs)
	{
		std::cerr << "error oppenning new file \n";
		_response.status_line = "HTTP/1.1 403 Forbidden";
		_response.body = get_body_error(403);
	}
	offset = pos + 1;
	pos = img.find("\r\n\r\n", offset);
	if (pos == std::string::npos){ return ;}
	offset = pos + 4;
	content = img.substr(offset, img.size() - offset);
	ofs << content;
	if (chdir("../.."))
		std::cerr << "CHDIR FAILED\n";
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

	//probablement ajouter un truc qui clear la requete si deja remplie avant de la remplir
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
