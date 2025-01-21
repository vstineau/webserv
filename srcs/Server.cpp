
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
	_conf.server_name = "webserv";
	_conf.len_address = sizeof(address);
}

Server::Server(config conf):	server_fd(0),
															client_fd(0),
															_conf(conf)
{}

Server::~Server()
{}


void Server::fillRequest(int n, std::string &buffer)
{
	size_t pos = 0;
	size_t offset = 0;
	std::string key;

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

	pos = buffer.find("\r\n", offset);
	_requests[n].url = buffer.substr(offset, pos - offset);
	offset = pos + 2;
	while (pos != std::string::npos)
	{
		pos = buffer.find(":", offset);
		if (pos == std::string::npos){ break;}
		key = buffer.substr(offset, pos - offset);
		offset = pos + 1;
		pos = buffer.find("\r\n", offset);
		if (pos == std::string::npos){ break;}
		_requests[n].headers[key] = buffer.substr(offset, pos - offset);
		offset = pos + 1;
		if (!buffer.find("\r\n\r\n", offset -1))
			break;
	}
	offset += 2;
	_requests[n].body = buffer.substr(offset, buffer.size() - offset);
}

//void Server::setRequest(void)
//{
//	char buff[1024] = {0};
//	request	request;
//	int bytes_red;
//	std::string buffer;
//	
//	bytes_red = recv(client_fd, buff, 1024, 0);
//	if (bytes_red < 0)
//	{
//		perror("recv");
//	}
//	buffer = buff;
//	std::cout << buffer << std::endl;
//	std::string http_response =
//	"HTTP/1.1 200 OK\r\n"  // Ligne de statut HTTP
//	"Content-Type: text/html\r\n"  // Type de contenu (HTML)
//	"\r\n"  // Fin des en-têtes
//	"<html><body><h1>AAAAAAAAAAAA Webserv</h1></body></html>";  // Corps de la réponse
//	send(client_fd, http_response.c_str(), http_response.size(), 0);
//	fillRequest(request, buffer);
//	_requests[client_fd] = request;
//	print_request(request);
//  close(client_fd);  // Ferme la connexion avec le client
//  close(server_fd);  // Ferme la connexion avec le client
//}
//

//link server socket to the IP adress and to the port(s), start listenning then accept connection from client
int Server::bindListenAccept()
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
