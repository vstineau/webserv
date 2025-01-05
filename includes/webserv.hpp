//https://www.ietf.org/rfc/rfc2068.txt
//https://trungams.github.io/2020-08-23-a-simple-http-server-from-scratch/
#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <cstdlib>

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

struct config {
	std::string								server_name;
	std::string								host;
	std::string								http_redirection;
	std::string								directory_path;
	std::vector<std::string>	http_methods_allowed;;
	std::vector<std::string>	port;
	int												client_body_size;
	bool											directory_listing;
};

struct request {
};

class Server {

public :
	Server();
	Server(config conf_file);
	~Server();

private :
	const config	conf;
	std::map<int, request> requests;
};

#endif
