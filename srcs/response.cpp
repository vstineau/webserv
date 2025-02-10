#include "../includes/webserv.hpp"

std::ostream & operator<<(std::ostream & o, response & i)
{
	o << i.version << i.status_code << "\r\n";
	for (std::map<std::string, std::string>::iterator it = i.headers.begin(); it != i.headers.end(); it++)
		o << it->first << it->second << "\r\n";
	o << "\r\n";
	o << i.body;
	return (o);
}
