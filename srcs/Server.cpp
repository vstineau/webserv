
#include "../includes/Server.hpp"
#include "../includes/color.hpp"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

Server::Server() :	_server_fd(0),
										_client_fd(0),
										_default_conf(true),
										_address()
{
	_conf.server_name = "webserv";
	_conf.len_address = sizeof(_address);
}

Server::Server(config conf):	_conf(conf),
															_server_fd(0),
															_client_fd(0),
															_default_conf(false)
{}

Server::~Server()
{}


void Server::_fillRequest(request &request, std::string &buffer)
{
	size_t pos = 0;
	size_t offset = 0;
	std::string key;

	if (!buffer.find("GET"))
	{
		request.method = GET;
		offset = pos + 3;
	}
	else if (!buffer.find("POST"))
	{
		request.method = POST;
		offset = pos + 4;
	}
	else if (!buffer.find("DELETE"))
	{
		request.method = DELETE;
		offset = pos + 6;
	}

	pos = buffer.find("\r\n", offset);
	request.url = buffer.substr(offset, pos - offset);
	offset = pos + 2;
	while (pos != std::string::npos)
	{
		pos = buffer.find(":", offset);
		if (pos == std::string::npos){ break;}
		key = buffer.substr(offset, pos - offset);
		offset = pos + 1;
		pos = buffer.find("\r\n", offset);
		if (pos == std::string::npos){ break;}
		request.headers[key] = buffer.substr(offset, pos - offset);
		offset = pos + 1;
		if (!buffer.find("\r\n\r\n", offset -1))
			break;
	}
	offset += 2;
	request.body = buffer.substr(offset, buffer.size() - offset);
}

void Server::setRequest(void)
{
	char buff[1024] = {0};
	request	request;
	int bytes_red;
	std::string buffer;
	
	bytes_red = recv(_client_fd, buff, 1024, 0);
	if (bytes_red < 0)
	{
		perror("recv");
	}
	buffer = buff;
	std::cout << buffer << std::endl;
	std::string http_response =
	"HTTP/1.1 200 OK\r\n"  // Ligne de statut HTTP
	"Content-Type: text/html\r\n"  // Type de contenu (HTML)
	"\r\n"  // Fin des en-têtes
	"<html><body><h1>AAAAAAAAAAAA Webserv</h1></body></html>";  // Corps de la réponse
	send(_client_fd, http_response.c_str(), http_response.size(), 0);
	_fillRequest(request, buffer);
	_requests[_client_fd] = request;
	print_request(request);
  close(_client_fd);  // Ferme la connexion avec le client
  close(_server_fd);  // Ferme la connexion avec le client
}

//create a socket for the serveur
int	Server::setServFd()
{
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd < 0)
	{
		perror("socket");
		return (1);
	}
	return (0);
}

//link server socket to the IP adress and to the port(s), start listenning then accept connection from client
int Server::bindListenAccept()
{
	struct sockaddr_in			addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;//inet_addr("0.0.0.0");
	addr.sin_port = htons(8080);
	_address = addr;

	int b = true;
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd == -1) {
		perror("socket");
	}
	setsockopt(_server_fd, SOL_SOCKET, SO_REUSEPORT, &b, sizeof(int));
	if (bind(_server_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
	{
		perror("bind");
		close(_server_fd);
		return (1);
	}
	if (listen(_server_fd, 10) == -1)
	{
		perror("listen");
		close(_server_fd);
		return (1);
	}
	//std::cout << "Server currently listenning on port : " << _conf.port << "\n";
	_client_fd = accept(_server_fd, (struct sockaddr *)&_address, (socklen_t *)&_conf.len_address);
	if (_client_fd < 0)
	{
		perror("accept");
		return (1);
	}
	std::cout << "Client connected\n";
	return (0);
}
