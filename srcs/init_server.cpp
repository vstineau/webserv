
#include "../includes/Server.hpp"
#include "../includes/webserv.hpp"

void init_server(std::map<int, Server> &map_serv, std::vector<config> &confs)
{
	for (int i = 0; i < confs.size(); i++)
	{
		Server temp(confs[i]);
		map_serv[temp.setServer_fd()] = temp;
	}
}
