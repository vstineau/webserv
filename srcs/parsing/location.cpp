
#include "../../includes/webserv.hpp"

static void	get_return(std::string &path, std::string &buffer,  config &conf)
{
	size_t				pos = 0;
	size_t				offset = 0;

	pos = buffer.find("return 301 ");
	if (pos == std::string::npos)
		return ;
	offset = pos + 12;
	pos = buffer.find("\n", offset);
	if (pos == std::string::npos)
		return ;
	conf.locations[path].ret = buffer.substr(offset, pos - offset);
}

static void	get_cgi_extention(std::string &path, std::string &buffer,  config &conf)
{
	size_t				pos = 0;
	size_t				offset = 0;

	pos = buffer.find("cgi_extention = ");
	if (pos == std::string::npos)
		return ;
	offset = pos + 17;
	pos = buffer.find("\n", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no cgi_extention found\n";
		return ;
	}
	conf.locations[path].cgi_extention = buffer.substr(offset, pos - offset);
}

static void	get_cgi_bin(std::string &path, std::string &buffer,  config &conf)
{
	size_t				pos = 0;
	size_t				offset = 0;

	pos = buffer.find("cgi_bin = ");
	if (pos == std::string::npos)
		return ;
	offset = pos + 11;
	pos = buffer.find("\n", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no cgi_bin found\n";
		return ;
	}
	conf.locations[path].cgi_bin = buffer.substr(offset, pos - offset);
}

static void	get_root(std::string &path, std::string &buffer,  config &conf)
{
	size_t				pos = 0;
	size_t				offset = 0;

	pos = buffer.find("root = ");
	if (pos == std::string::npos)
		return ;
	offset = pos + 8;
	pos = buffer.find("\n", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no root found\n";
		return ;
	}
	conf.locations[path].root = buffer.substr(offset, pos - offset);
}

static void	get_method_allowed(std::string &path, std::string &buffer,  config &conf)
{
	size_t				pos = 0;
	size_t				offset = 0;
	size_t				wc = 0;
	std::string		methods;

	pos = buffer.find("methods = ");
	if (pos == std::string::npos)
		return ;
	offset = pos + 11;
	pos = buffer.find(";", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no methods found\n";
		return ;
	}
	methods = buffer.substr(offset, (pos - 1) - offset);
	if (methods.empty())
		return ;
	offset = 0;
	wc = count_words(methods.c_str(), ' ');
	for(size_t i = 0; i < wc; i++)
	{
		pos = methods.find(" ");
		if (pos == std::string::npos)
		{
			set_method(conf.locations[path], methods.substr(offset, methods.size() - offset));
		}
		else
		{
			set_method(conf.locations[path], methods.substr(offset, methods.size() - offset));
			offset = pos + 1;
		}
	}
}

static void	set_error_pages(std::string &path, std::string &buffer,  config &conf)
{
	size_t				pos = 0;
	size_t				offset = 0;
	int						error_num = 0;

	while (pos != std::string::npos)
	{
		pos = buffer.find("error_pages ");
		if (pos == std::string::npos)
			return ;
		offset = pos + 13;
		pos = buffer.find(" ");
		if (pos == std::string::npos)
			continue ;
		error_num = atoi(buffer.substr(offset, pos - offset).c_str());
		pos = buffer.find("= ");
		offset = pos + 2;
		if (pos == std::string::npos)
			continue ;
		pos = buffer.find("\n", offset);
		if (pos == std::string::npos)
			return ;
		conf.locations[path].error_pages[error_num] = buffer.substr(offset, pos - offset);
	}
}

static void	get_client_body_size(std::string &path, std::string &buffer,  config &conf)
{
	size_t				pos = 0;
	size_t				offset = 0;

	pos = buffer.find("client_body_size = ");
	if (pos == std::string::npos)
		return ;
	offset = pos + 20;
	pos = buffer.find("\n", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no client_body_size found\n";
		return ;
	}
	conf.locations[path].client_body_size = atoi(buffer.substr(offset, pos - offset).c_str());
}

void	fill_location(std::string &path, std::string &buffer, config &conf)
{
	get_return(path, buffer, conf);
}

void get_locations_bloc(std::string &buffer, config &conf)
{
	std::string		path;
	std::string		location_string;
	size_t				pos = 0;
	size_t				offset = 0;

	while (pos != std::string::npos)
	{
		pos = buffer.find("location ");
		if (pos == std::string::npos)
			return ;
		offset = pos + 10;
		pos = buffer.find("{");
		if (pos == std::string::npos)
			continue ;
		path = buffer.substr(offset, (pos - 1) - offset);
		pos = buffer.find("\n");
		if (pos == std::string::npos)
			continue ;
		offset = pos + 1;
		pos = buffer.find("}\n", offset);
		if (pos == std::string::npos)
			return ;
		location_string = buffer.substr(offset, pos - offset);
		fill_location(path, location_string, conf);
		offset = pos + 2;
	}


}

