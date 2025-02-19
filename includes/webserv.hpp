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
#include <sstream>
#include <cstdlib>
#include <cstddef>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/socket.h>
#include "color.hpp"
#include "template_utils.hpp"

#define MAX_EVENTS 28
#define ENDL "\r\n"

extern bool g_end;

class BadConfigFileExeption : public std::exception
{
	public :
		virtual const char *what() const throw()
		{
			return "Error: invalid configuration file\n";
		}
};

enum methods
{
	GET,
	POST,
	DELETE,
	INVALID_METHOD
};

struct location
{
	location();
	~location();
	std::string									ret;
	std::string									root;
	std::string									cgi_extention;
	std::string									cgi_bin;
	char												allowed_method[4];
	int													client_body_size;
	bool														directory_listing;
	std::map<int, std::string>	error_pages;
};

struct config {
	config();
	~config();
	std::vector<std::string>				server_names;
	std::vector<std::string>				server_index;
	std::string											host;
	std::string											http_redirection;
	std::string											directory_path;
	std::map<int, std::string>			error_pages;
	char														allowed_method[4];
	int															port;
	int															client_body_size;
	bool														directory_listing;
	std::map<std::string, location>	locations; //PATH -> location 
};

struct response {
	std::string	status_line;
	std::map<std::string, std::string> headers;
	std::string body;
};

struct request {
	std::string path;
	std::string version;
	methods method;
	std::map<std::string, std::string> headers;
	std::string body; //pas de c_str() parce qu'il peut y avoir de s\0 qui se baladent
	std::string getContentType(std::string &buffer) const;
	unsigned int getContentLength(std::string &buffer) const;
};
//connection: keep-alive -> reply everytime with connection: keep-alive if connection: close quitter tout 
class Server;

//EPOLL STUFF
void epoll_loop(Server &serv);

//PARSING OF THE CONFIGURATON FILE
size_t		count_words(const char *line, char c);
size_t		how_many_serv(char *file);
void			get_one_config(config &conf, std::string &buffer);
void			fill_servers_configs(std::vector<config> &confs, char *file);
void			set_method(location &loc, std::string method);
void			get_locations_bloc(config &conf, std::string &buffer);

//RESPONSES
void				file_in_string(std::string &sfile, char *file);
std::string	get_body_error(int status_code);

//SIGNAL
int				init_signals(void);

//DIRECTORY LISTING
std::string	directory_listing(std::string path);
std::string	upload(std::string path);
std::string	fillDirectoryListing(std::string listing);

//DEBUG
void	print_config(std::vector<config> &confs);


//#endif
