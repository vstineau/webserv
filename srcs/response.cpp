#include "../includes/webserv.hpp"

std::ostream & operator<<(std::ostream & o, response const & i)
{
	o << i.version << i.status_code << i.headers << i.body;
	return (o);
}
