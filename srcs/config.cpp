#include "../includes/webserv.hpp"

config::config() : port(0), client_body_size(0), directory_listing(false)
{
	for (int i = 0; i < 4; i++)
		allowed_method[i] = 0;
}

config::~config()
{
}

config &config::operator=(config &c)
{
	server_name = c.server_name;
	server_index = c.server_index;
	root = c.root;
	host = c.host;
	upload_directory = c.upload_directory;
	allowed_method[0] = c.allowed_method[0];
	allowed_method[1] = c.allowed_method[1];
	allowed_method[2] = c.allowed_method[2];
	allowed_method[3] = c.allowed_method[3];
	client_body_size = c.client_body_size;
	directory_listing = c.directory_listing;
	error_pages = c.error_pages;//
	locations = c.locations;
	port = c.port;

	return *this;
}