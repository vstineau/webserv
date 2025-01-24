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

void	get_server_port(config &conf, std::string &buffer)
{
	size_t				pos = 0;
	size_t				offset = 0;

	pos = buffer.find("port = ");
	offset = pos + 8;
	pos = buffer.find("\n", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no port found\n";
		return ;
	}
	conf.port = atoi(buffer.substr(offset, pos - offset).c_str());
}

void	get_server_name(config &conf, std::string &buffer)
{
	size_t				pos = 0;
	size_t				offset = 0;
	size_t				wc = 0;
	std::string		names;

	pos = buffer.find("server_name = ");
	offset = pos + 15;
	pos = buffer.find(";", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no server name found\n";
		return ;
	}
	names = buffer.substr(offset, (pos - 1) - offset);
	offset = 0;
	wc = count_words(names.c_str(), ' ');
	for(size_t i = 0; i < wc; i++)
	{
		pos = names.find(" ");
	if (pos == std::string::npos)
		conf.server_names[i] = names.substr(offset, names.size() - offset);
	else
		{
			conf.server_names[i] = names.substr(offset, pos - offset);
			offset = pos + 1;
		}
	}
}

void	get_index(config &conf, std::string &buffer)
{
	size_t				pos = 0;
	size_t				offset = 0;
	size_t				wc = 0;
	std::string		index;

	pos = buffer.find("index = ");
	offset = pos + 9;
	pos = buffer.find(";", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "no server name found\n";
		return ;
	}
	index = buffer.substr(offset, (pos - 1) - offset);
	offset = 0;
	wc = count_words(index.c_str(), ' ');
	for(size_t i = 0; i < wc; i++)
	{
		pos = index.find(" ");
	if (pos == std::string::npos)
		conf.server_index[i] = index.substr(offset, index.size() - offset);
	else
		{
			conf.server_index[i] = index.substr(offset, pos - offset);
			offset = pos + 1;
		}
	}
}

void	get_one_config(config &conf, std::string &buffer)
{
	get_server_name(conf, buffer);
	get_server_port(conf, buffer);
	
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
