
#include "../includes/webserv.hpp"
#include "../includes/Init.hpp"

Init::Init()
{}

Init::~Init()
{}

void Init::fill_servers_configs(const std::string &file)
{
	std::string line;
	std::string serv_line;
	std::ifstream ifs(file.c_str());
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

void Init::init_servers()
{
	for (std::size_t i = 0; i < confs.size(); i++)
	{
		Server temp(confs[i]);
		servs.push_back(temp);
		servs[i].setSocket();
	}
}
