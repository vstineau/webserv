#include "../includes/Server.hpp"

void	Server::print_request(int n)
{
	if (_requests[n].method == GET)
		std::cout << "method : GET\n";
	if (_requests[n].method == POST)
		std::cout << "method : POST\n";
	if (_requests[n].method == DELETE)
		std::cout <<  "method : DELETE\n";
	std::cout << "PATH = " << _requests[n].path << "\n";
	std::cout << "VERSION = " << _requests[n].version << "\n";
	for (std::map<std::string, std::string>::iterator it = _requests[n].headers.begin(); it != _requests[n].headers.end(); it++)
		std::cout << it->first << " = "  << it->second << "\n\n";
	// std::cout << "BODY = " << _requests[n].body << "\n";
}

void	Server::print_response(response &rep)
{
	std::cout << B_GREEN << "VERSION = " << rep.status_line << "\n";
	for (std::map<std::string, std::string>::iterator it = _response.headers.begin(); it != _response.headers.end(); it++)
		std::cout << it->first << " = "  << it->second << "\n\n";
	std::cout << "BODY = " << rep.body << RESET << "\n";
}

std::ostream & operator<<(std::ostream & o, location & i)
{
	o << "		RETURN : " << i.ret << "\n";
	o << "		ROOT : " << i.root << "\n";
	o << "		CGI EXTENTION : " << i.cgi_extention << "\n";
	o << "		CGI BIN : " << i.cgi_bin << "\n";
	o << "		CLIENT BODY SIZE : " << i.client_body_size << "\n";
	o << "		DIRECTORY LISTING : " << i.directory_listing << "\n";
	o << "		METHODS ALLOWED : ";
	if (i.allowed_method[GET])
		o << "GET ";
	if (i.allowed_method[POST])
		o << "POST ";
	if (i.allowed_method[DELETE])
		o << "DELETE ";
	o << "\n";
	o << "		ERROR PAGES : \n";
	for (std::map<int, std::string>::iterator im = i.error_pages.begin(); im != i.error_pages.end(); im++)
	{
		o << "		" << im->first << " " << im->second << "\n";
	}
	return (o);
}

std::ostream & operator<<(std::ostream & o, config & i)
{
	o << "SERVER NAMES : " << i.server_names.size() << "\n";
	for (std::size_t c = 0; c < i.server_names.size(); c++)
		o << i.server_names[c] << " ";
	o << "\n";
	o << "SERVER INDEX : " << i.server_index.size() << "\n";
	for (std::vector<std::string>::iterator it = i.server_index.begin(); it != i.server_index.end(); it++)
		o << *it << " ";
	o << "\n";
	o << "HOST : " << i.host << "\n";
	o << "PORT : " << i.port << "\n";
	o << "ROOT : " << i.root << "\n";
	o << "CLIENT BODY SIZE : " << i.client_body_size << "\n";
	o << "DIRECTORY LISTING : " << i.directory_listing << "\n";
	o << "ERROR PAGES : \n";
	for (std::map<int, std::string>::iterator im = i.error_pages.begin(); im != i.error_pages.end(); im++)
	{
		o << im->first << " " << im->second << "\n";
	}
	for (std::map<std::string, location>::iterator im = i.locations.begin(); im != i.locations.end(); im++)
	{
		o << "	LOCATION PATH : " << im->first << "\n" << "	LOCATION DETAILS :\n";
		o << im->second;
	}
	return (o);
}
void	print_config(std::vector<config> &confs)
{
	for (std::vector<config>::iterator it = confs.begin(); it != confs.end(); it++)
	{
		std::cout << *it << std::endl;
	}
}
