//https://www.ietf.org/rfc/rfc2068.txt
//https://trungams.github.io/2020-08-23-a-simple-http-server-from-scratch/
//https://devarea.com/linux-io-multiplexing-select-vs-poll-vs-epoll/
//https://man7.org/linux/man-pages/man7/epoll.7.html

#pragma once
//#ifndef WEBSERV_HPP
//#define WEBSERV_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <cstdlib>
#include <cstddef>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/socket.h>
#include "color.hpp"
#include "parsing.hpp"

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

#define MAX_EVENTS 28


class BadConfigFileExeption : public std::exception
{
	public :
		virtual const char *what() const throw()
		{
			return "Error: invalid configuration file\n";
		}
};

enum method
{
	GET,
	POST,
	DELETE,
};

struct location
{
	std::string ret;
	std::string root;
	std::string cgi_extention;
	std::string cgi_bin;
	char	allowed_method[3];
	std::vector<std::string> error_pages;
};

struct config {
	std::vector<std::string>	server_names;
	std::vector<std::string>	server_index;
	std::string								host;
	std::string								http_redirection;
	std::string								directory_path;
	std::string								error_pages;
	char											allowed_method[3];
	int												port;
	int												client_body_size;
	bool											directory_listing;
	std::map<std::string, location> locations; //PATH -> location 
};

struct request {
	std::string path;
	std::string version;
	method method;
	std::map<std::string, std::string> headers;
	std::string body; //pas de c_str() parce qu'il peut y avoir de s\0 qui se baladent
	std::string getContentType(std::string &buffer) const;
	unsigned int getContentLength(std::string &buffer) const;
};

struct response {
	std::string version;
	int	status_code;
	std::string message;
	std::map<std::string, std::string> headers;
	std::string body;
};
//connection: keep-alive -> reply everytime with connection: keep-alive if connection: close quitter tout 
class Server;

//EPOLL STUFF
void epollinit(Server &serv);
void epoll_loop(Server &serv, struct epoll_event &ev, struct epoll_event events[MAX_EVENTS], int epoll_fd);

//PARSING OF THE CONFIGURATON FILE
size_t count_words(std::string line, char c);
size_t	how_many_serv(char *file);
void fill_servers_configs(std::vector<config> &confs, char *file);

//#endif
