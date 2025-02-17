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
	std::cout << "BODY = " << _requests[n].body << "\n";
}

void	Server::print_response(response &rep)
{
	std::cout << B_GREEN << "VERSION = " << rep.status_line << "\n";
	for (std::map<std::string, std::string>::iterator it = _response.headers.begin(); it != _response.headers.end(); it++)
		std::cout << it->first << " = "  << it->second << "\n\n";
	std::cout << "BODY = " << rep.body << RESET << "\n";
}
