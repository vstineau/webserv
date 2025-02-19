
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

	pos = buffer.find("cgi-extention: ");
	if (pos == std::string::npos)
		return ;
	offset = pos + 15;
	pos = buffer.find(";", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no cgi-extention found\n";
		return ;
	}
	conf.locations[path].cgi_extention = buffer.substr(offset, pos - offset);
}

static void	get_cgi_bin(std::string &path, std::string &buffer,  config &conf)
{
	size_t				pos = 0;
	size_t				offset = 0;

	pos = buffer.find("cgi-bin: ");
	if (pos == std::string::npos)
		return ;
	offset = pos + 9;
	pos = buffer.find(";", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no cgi-bin found\n";
		return ;
	}
	conf.locations[path].cgi_bin = buffer.substr(offset, pos - offset);
}

static void	get_root(std::string &path, std::string &buffer,  config &conf)
{
	size_t				pos = 0;
	size_t				offset = 0;

	pos = buffer.find("root: ");
	if (pos == std::string::npos)
		return ;
	offset = pos + 6;
	pos = buffer.find(";", offset);
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

	pos = buffer.find("methods: ");
	if (pos == std::string::npos)
		return ;
	offset = pos + 9;
	pos = buffer.find(";", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no methods found\n";
		return ;
	}
	methods = buffer.substr(offset, pos - offset);
	if (methods.empty())
		return ;
	offset = 0;
	wc = count_words(methods.c_str(), ' ');
	for(size_t i = 0; i < wc; i++)
	{
		pos = methods.find(" ", offset);
		if (pos == std::string::npos)
		{
			set_method(conf.locations[path], methods.substr(offset, methods.size() - offset));
		}
		else
		{
			set_method(conf.locations[path], methods.substr(offset, pos - offset));
			offset = pos + 1;
		}
	}
}

static void	get_client_body_size(std::string &path, std::string &buffer,  config &conf)
{
	size_t				pos = 0;
	size_t				offset = 0;

	pos = buffer.find("client-body-size: ");
	if (pos == std::string::npos)
		return ;
	offset = pos + 18;
	pos = buffer.find(";", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no client-body-size found\n";
		return ;
	}
	conf.locations[path].client_body_size = atoi(buffer.substr(offset, pos - offset).c_str());
}

void	fill_location(std::string &path, std::string &buffer, config &conf)
{
	get_return(path, buffer, conf);
	get_client_body_size(path, buffer, conf);
	get_root(path, buffer, conf);
	get_cgi_bin(path, buffer, conf);
	get_cgi_extention(path, buffer, conf);
	get_method_allowed(path, buffer, conf);
}

void get_locations_bloc(config &conf, std::string &buffer)
{
	std::string		path;
	std::string		location_string;
	size_t				pos = 0;
	size_t				offset = 0;

	while (pos != std::string::npos)
	{
		pos = buffer.find("location ", offset);
		if (pos == std::string::npos)
			return ;
		offset = pos + 10;
		pos = buffer.find("{", offset);
		if (pos == std::string::npos)
			continue ;
		path = buffer.substr(offset, pos - offset);
		pos = buffer.find("\n", offset);
		if (pos == std::string::npos)
			continue ;
		offset = pos + 1;
		pos = buffer.find("}\n", offset);
		if (pos == std::string::npos)
			return ;
		location_string = buffer.substr(offset, pos - offset);
		fill_location(path, location_string, conf);
		offset = pos + 2;
		//std::cout << RED << "ICI WHILE 1" << RESET << std::endl;
	}


}

//static void	set_error_pages(std::string &path, std::string &buffer,  config &conf)
//{
//	size_t				pos = 0;
//	size_t				offset = 0;
//	int						error_num = 0;
//
//	while (pos != std::string::npos)
//	{
//		pos = buffer.find("error_pages ");
//		if (pos == std::string::npos)
//			return ;
//		offset = pos + 13;
//		pos = buffer.find(" ");
//		if (pos == std::string::npos)
//			continue ;
//		error_num = atoi(buffer.substr(offset, pos - offset).c_str());
//		pos = buffer.find("= ");
//		offset = pos + 2;
//		if (pos == std::string::npos)
//			continue ;
//		pos = buffer.find("\n", offset);
//		if (pos == std::string::npos)
//			return ;
//		conf.locations[path].error_pages[error_num] = buffer.substr(offset, pos - offset);
//	}
//}

