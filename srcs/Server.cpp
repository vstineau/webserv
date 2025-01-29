
#include "../includes/Server.hpp"
#include "../includes/color.hpp"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
/*
 * parser les chunk quand multipart/form-data dans content type en utilisant le boundary chaque chunk = un fichier creer dans www/import/ (ofs)
std::ofstream ofs(FileName.c_str(), std::ios_base::binary);
POST / HTTP/1.1
Host: localhost:9999
User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:134.0) Gecko/20100101 Firefox/134.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,/;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br, zstd
Content-Type: multipart/form-data; boundary=---------------------------14088831271989056644481321813
Content-Length: 198889
Origin: http://localhost:9999/
Connection: keep-alive
Referer: http://localhost:9999/
Upgrade-Insecure-Requests: 1
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: same-origin
Sec-Fetch-User: ?1
Priority: u=0, i

-----------------------------14088831271989056644481321813
Content-Disposition: form-data; name="file"; filename="vstineau.jpg"
Content-Type: image/jpeg
*/
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
{}

void		Server::SetResponse(void)
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

	pos = buffer.find("/", offset);
	if (pos == std::string::npos){ return;}
	offset = pos;
	pos = buffer.find(" ", offset);
	if (pos == std::string::npos){ return;}
	_requests[n].path = buffer.substr(offset, pos - offset);
	offset = pos + 1;
	pos = buffer.find("\r\n", offset);
	if (pos == std::string::npos){ return;}
	_requests[n].version = buffer.substr(offset, pos - offset);
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
	_requests[n].body = buffer.substr(offset, buffer.size());// - offset);
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
