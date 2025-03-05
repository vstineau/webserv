
#include "../includes/Init.hpp"
#include "../includes/Server.hpp"
#include "../includes/webserv.hpp"
#include <map>

bool g_end;

int main(int argc, char *argv[]) {
	g_end = false;
	init_signals();
	std::string file;
	Init webserv;

	// std::vector<config> confs;
	// std::map<int, Server> map_serv;
	if (argc == 1)
	{
		file = default_conf;
		webserv.fill_servers_configs(file);
	}
	else if (argc == 2)
	{
		file = argv[1];
		webserv.fill_servers_configs(argv[1]);
	}
	webserv.init_servers();
	// print_config(webserv.confs);
	// init_server(map_serv, confs);
	webserv.epoll_loop();
}
