#include "../includes/Server.hpp"

void	Server::print_request(int n)
{
	if (_requests[n].method == GET)
		std::cout << B_GREEN << "method : GET\n";
	if (_requests[n].method == POST)
		std::cout << B_GREEN << "method : POST\n";
	if (_requests[n].method == DELETE)
		std::cout << B_GREEN << "method : DELETE\n";
	std::cout << "PATH = " << _requests[n].path << "\n";
	std::cout << "VERSION = " << _requests[n].version << "\n";
	for (std::map<std::string, std::string>::iterator it = _requests[n].headers.begin(); it != _requests[n].headers.end(); it++)
		std::cout << B_BLUE << it->first << " = " << it->second << "\n";
	std::cout << B_RED << "BODY = " << _requests[n].body << RESET << "\n";
}

void	Server::print_response(response &rep)
{
	std::cout << B_GREEN << "VERSION = " << rep.version << "\n";
	std::cout << "STATUS CODE = " << rep.status_code << "\n";
	for (std::map<std::string, std::string>::iterator it = rep.headers.begin(); it != rep.headers.end(); it++)
		std::cout << B_BLUE << it->first << " = " << it->second << "\n";
	std::cout << B_RED << "MESSAGE = " << rep.message << "\n";
	std::cout << "BODY = " << rep.body << RESET << "\n";
}
