//https://docs.nginx.com/nginx/admin-guide/web-server/web-server/
//https://www.plesk.com/blog/various/nginx-configuration-guide/
//https://www.digitalocean.com/community/tutorials/understanding-the-nginx-configuration-file-structure-and-configuration-contexts
#include "../../includes/webserv.hpp"

void default_configuration(config &conf)
{
	conf.server_name = "webserv";
	conf.host = "127.0.0.1:8080";
	conf.http_redirection= "none";
	conf.http_methods_allowed = {GET, POST, DELETE};
}

void	fill_config(config &conf, char *file)
{
	std::ifstream ifs(file);

	if (!ifs)
	{
		std::cerr << "no valid configuration file provided, default configuration will be used\n";
		default_configuration(conf);
		return ;
	}

}
