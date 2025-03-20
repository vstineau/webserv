#include "../includes/webserv.hpp"

location::location() : client_body_size(0), directory_listing(false)
{
	for (int i = 0; i < 4; i++)
		allowed_method[i] = 0;
}

location::~location()
{
}

location &location::operator=(location &l)
{
	ret = l.ret;
	root = l.root;
	cgi_extention = l.cgi_extention;
	cgi_bin = l.cgi_bin;
	index_html = l.index_html;
	upload_directory = l.upload_directory;
	allowed_method[0] = l.allowed_method[0];
	allowed_method[1] = l.allowed_method[1];
	allowed_method[2] = l.allowed_method[2];
	allowed_method[3] = l.allowed_method[3];
	client_body_size = l.client_body_size;
	directory_listing = l.directory_listing;
	// error_pages = l.error_pages;//

	return *this;
}