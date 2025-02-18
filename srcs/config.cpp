#include "../includes/webserv.hpp"

config::config():	port(0),
									client_body_size(0),
									directory_listing(false)
{
	for (int i = 0; i < 4; i++)
		allowed_method[i] = 0;
}

config::~config()
{}
