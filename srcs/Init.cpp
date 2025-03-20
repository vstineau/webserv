
#include "../includes/webserv.hpp"
#include "../includes/Init.hpp"

Init::Init()
{}

Init::~Init()
{}

void Init::set_default_conf()
{
	config temp;

	temp.server_name = "webserv";
	temp.root = "www";
	temp.locations[""].root = temp.root;
	temp.locations[""].directory_listing = true;
	set_method(temp, "DEFAULT");
	set_method(temp.locations[""], "DEFAULT");
	temp.port = 8080;
	temp.directory_listing = true;
	confs.push_back(temp);
}

void Init::fill_servers_configs(char *file)
{
	std::string line;
	std::string serv_line;
	std::ifstream ifs(file);
	if (!ifs)
		throw BadConfigFileExeption();
	confs.reserve(how_many_serv(file));
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
			for(std::vector<config>::iterator it = confs.begin(); it != confs.end(); it++)
			{
				if (temp.server_name == it->server_name)
					throw Init::BadConfigFileExeption();
			}
			confs.push_back(temp);
			serv_line.clear();
		}
	}
}

void Init::init_servers()
{
	for (std::size_t i = 0; i < confs.size(); i++)
	{
		Server temp(confs[i]);
		servs.push_back(temp);
		servs[i].setSocket();
	}
}
