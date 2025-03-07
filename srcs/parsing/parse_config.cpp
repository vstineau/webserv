//https://docs.nginx.com/nginx/admin-guide/web-server/web-server/
//https://www.plesk.com/blog/various/nginx-configuration-guide/
//https://www.digitalocean.com/community/tutorials/understanding-the-nginx-configuration-file-structure-and-configuration-contexts
#include "../../includes/webserv.hpp"

size_t	how_many_serv(char *file)
{
	std::string line;
	std::ifstream ifs(file);
	size_t count = 0;
	if (!ifs)
		return (std::cerr << "Error: can't open configuration file\n", 1); //so run server like there is no configuration file
	while (std::getline(ifs, line))
	{
		if (line == "Server {" )
			count ++;
	}
	return (count);
}

void	fill_servers_configs(std::vector<config> &confs, char *file)
{
	std::string line;
	std::string serv_line;
//	size_t serv_index = 0;
	std::ifstream ifs(file);
	confs.reserve(how_many_serv(file));
	if (!ifs)
		(NULL);
	while (std::getline(ifs, line))
	{
		if (line == "Server {" )
		{
			config temp;
			while (line != "}")
			{
				std::getline(ifs, line);
				if (line != "}")
				{
					serv_line += line;
					serv_line += "\n";
				}
			}
			get_one_config(temp, serv_line);
			confs.push_back(temp);
			serv_line.clear();
		}
	}
}

static void	set_error_pages(config &conf, std::string &buffer)
{
	size_t				pos = 0;
	size_t				posloc = 0;
	size_t				offset = 0;
	int						error_num = 0;

	posloc = buffer.find("location");
	if (posloc == std::string::npos)
		;
	while (pos != std::string::npos && pos < posloc)
	{
		pos = buffer.find("error-page: ", offset);
		if (pos == std::string::npos || pos > posloc)
			break ;
		offset = pos + 12;
		pos = buffer.find(" ", offset);
		if (pos == std::string::npos)
			break ;
		error_num = atoi(buffer.substr(offset, pos - offset).c_str());
		pos = buffer.find(" ", offset);
		if (pos == std::string::npos)
			break ;
		offset = pos + 1;
		pos = buffer.find(";", offset);
		if (pos == std::string::npos)
			break ;
		conf.error_pages[error_num] = buffer.substr(offset, pos - offset);
	}
}

static void	set_root(config &conf, std::string &buffer)
{
	size_t				pos = 0;
	size_t				offset = 0;
	size_t				posloc = 0;

	posloc = buffer.find("location");
	if (posloc == std::string::npos)
		;
	pos = buffer.find("root: ");
	if (pos == std::string::npos || pos > posloc)
		return ;
	offset = pos + 6;
	pos = buffer.find(";", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no root found\n";
		return ;
	}
	conf.root = buffer.substr(offset, pos - offset);
}

static void	set_upload_directory(config &conf, std::string &buffer)
{
	size_t				pos = 0;
	size_t				offset = 0;
	size_t				posloc = 0;

	posloc = buffer.find("location");
	if (posloc == std::string::npos)
		;
	pos = buffer.find("upload: ");
	if (pos == std::string::npos || pos > posloc)
		return ;
	offset = pos + 8;
	pos = buffer.find(";", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no upload found\n";
		return ;
	}
	conf.upload_directory = buffer.substr(offset, pos - offset);
}

static void	get_server_port(config &conf, std::string &buffer)
{
	size_t				pos = 0;
	size_t				offset = 0;

	pos = buffer.find("port: ");
	if (pos == std::string::npos)
		return ;
	offset = pos + 6;
	pos = buffer.find(";", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no port found\n";
		return ;
	}
	conf.port = atoi(buffer.substr(offset, pos - offset).c_str());
}

static void	is_directory_listing_allowed(config &conf, std::string &buffer)
{
	size_t				pos = 0;
	size_t				offset = 0;
	size_t				posloc = 0;

	posloc = buffer.find("location");
	if (posloc == std::string::npos)
		;
	pos = buffer.find("directory-listing: ");
	if (pos == std::string::npos || pos > posloc)
		return ;
	offset = pos + 19;
	pos = buffer.find(";", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "unvalid config line found\n";
		return ;
	}
	if (buffer.substr(offset, pos - offset) == "on")
		conf.directory_listing = true;
	else
		conf.directory_listing = false;
}

static void	get_method_allowed(config &conf, std::string &buffer)
{
	size_t				pos = 0;
	size_t				offset = 0;
	size_t				wc = 0;
	std::string		methods;
	size_t				posloc = 0;

	posloc = buffer.find("location");
	if (posloc == std::string::npos)
		;
	pos = buffer.find("methods: ");
	if (pos == std::string::npos || pos > posloc)
	{
		set_method(conf, "DEFAULT");
		return ;
	}
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
			set_method(conf, methods.substr(offset, methods.size() - offset));
		}
		else
		{
			set_method(conf, methods.substr(offset, pos - offset));
			offset = pos + 1;
		}
	}
}

static void	get_server_name(config &conf, std::string &buffer)
{
	size_t				pos = 0;
	size_t				offset = 0;
	size_t				posloc = 0;

	posloc = buffer.find("location");
	if (posloc == std::string::npos)
		;
	pos = buffer.find("server-name: ");
	if (pos == std::string::npos || pos > posloc)
		return ;
	offset = pos + 13;
	pos = buffer.find(";", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no server-index found\n";
		return ;
	}
	conf.server_name = buffer.substr(offset, pos - offset);
}

static void	get_index(config &conf, std::string &buffer)
{
	size_t				pos = 0;
	size_t				offset = 0;
	size_t				posloc = 0;

	posloc = buffer.find("location");
	if (posloc == std::string::npos)
		;
	pos = buffer.find("index: ");
	if (pos == std::string::npos || pos > posloc)
		return ;
	offset = pos + 7;
	pos = buffer.find(";", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no index found\n";
		return ;
	}
	conf.server_index = buffer.substr(offset, pos - offset);
}

void	get_one_config(config &conf, std::string &buffer)
{
	get_server_name(conf, buffer);
	set_error_pages(conf, buffer);
	set_root(conf, buffer);
	get_index(conf, buffer);
	get_server_port(conf, buffer);
	is_directory_listing_allowed(conf, buffer);
	set_upload_directory(conf, buffer);
	get_locations_bloc(conf, buffer);
	get_method_allowed(conf, buffer);
}

//location = /


//void default_configuration(config &conf)
//{
//	conf.server_name = "webserv";
//	conf.host = "127.0.0.1:8080";
//	conf.http_redirection= "none";
//	conf.http_methods_allowed = {GET, POST, DELETE};
//}
//
//void	fill_config(config &conf, char *file)
//{
//	std::ifstream ifs(file);
//
//	if (!ifs)
//	{
//		std::cerr << "no valid configuration file provided, default configuration will be used\n";
//		default_configuration(conf);
//		return ;
//	}
//
//}
