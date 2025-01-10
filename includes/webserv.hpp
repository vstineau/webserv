//https://www.ietf.org/rfc/rfc2068.txt
//https://trungams.github.io/2020-08-23-a-simple-http-server-from-scratch/
//https://devarea.com/linux-io-multiplexing-select-vs-poll-vs-epoll/
//https://man7.org/linux/man-pages/man7/epoll.7.html

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

/*
III.3 Configuration file
You can get some inspiration from the ’server’ part of NGINX
configuration file.
In the configuration file, you should be able to:
• Choose the port and host of each ’server’.
• Setup the server_names or not.
• The first server for a host:port will be the default for this host:port (that means
it will answer to all the requests that don’t belong to an other server).
• Setup default error pages.
• Limit client body size.
• Setup routes with one or multiple of the following rules/configuration (routes wont
be using regexp):
◦ Define a list of accepted HTTP methods for the route.
◦ Define a HTTP redirection.
◦ Define a directory or a file from where the file should be searched (for example,
if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is
/tmp/www/pouic/toto/pouet).
◦ Turn on or off directory listing.
*/

class Server;

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
	struct sockaddr_in				address;
	int												len_address;
};

/*
    // 2. Définir l'adresse du socket (adresse IP et port)
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;  // Utilisation d'IPv4
    address.sin_addr.s_addr = INADDR_ANY;  // Écoute sur toutes les interfaces
    address.sin_port = htons(8080);  // Écoute sur le port 8080
*/

struct request {
	std::string url;
	method method; //get post delete
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
