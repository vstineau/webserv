#include "../includes/Server.hpp"

void	Server::print_request(request &req)
{
	if (req.method == GET)
		std::cout << B_GREEN << "method : GET\n";
	if (req.method == POST)
		std::cout << B_GREEN << "method : POST\n";
	if (req.method == DELETE)
		std::cout << B_GREEN << "method : DELETE\n";
	std::cout << "URL = " << req.url << "\n";
	for (std::map<std::string, std::string>::iterator it = req.headers.begin(); it != req.headers.end(); it++)
		std::cout << B_BLUE << it->first << " = " << it->second << "\n";
	std::cout << B_RED << "BODY = " << req.body << RESET << "\n";
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
