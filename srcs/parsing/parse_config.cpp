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

void	get_one_config(config &conf, std::string &buffer)
{
	size_t				pos = 0;
	size_t				offset = 0;
}

void fill_servers_configs(std::vector<config> &confs, char *file)
{
	std::ifstream	ifs(file);
	std::string		buffer;
	std::string		temp;

	for(size_t serv_index = 0; serv_index < confs.capacity(); serv_index++)
	{
		std::getline(ifs, buffer);
		while (buffer.empty() && buffer != "Server {")
			std::getline(ifs, buffer);
		//std::cout << BHI_RED << buffer << RESET << std::endl;
		if (buffer == "Server {")
		{
			buffer.clear();
			std::getline(ifs, temp);
			//std::cout << BHI_GREEN << temp << RESET << std::endl;
			while (temp != "}")
			{
				std::getline(ifs, temp);
				buffer += temp;
			}
		}
		std::cout << "_______CONF N." << serv_index << "________" << std::endl;
		std::cout << buffer << std::endl;
		std::cout << "_______________________" << std::endl;
		std::cout << std::endl;
		get_one_config(confs[serv_index], buffer);
	}
}
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
