
#include "../includes/webserv.hpp"
#include <map>
#include "../includes/Files.hpp"


Files::Files()
{
	mimes["jpg"]	= "image/jpeg";
	mimes["jpeg"]	= "image/jpeg";
	mimes["gif"]	= "image/gif";
	mimes["html"]	= "text/html";
	mimes["htm"]	= "text/html";
	mimes["css"]	= "text/css";
	mimes["json"] = "application/json";
	mimes["js"]		= "application/javascript";
	mimes["pdf"]	= "application/pdf";
	mimes["png"]	= "image/png";
	mimes["zip"]	= "application/zip";
	mimes["rar"]	= "application/x-rar-compressed";
	mimes["sh"]		= "application/x-sh";
}

//int stat(const char *restrict path, struct stat *restrict buf);
