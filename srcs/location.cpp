#include "../includes/webserv.hpp"

location::location():	client_body_size(0),
											directory_listing(false)
{
	for (int i = 0; i < 4; i++)
		allowed_method[i] = 0;
}

location::~location()
{}
