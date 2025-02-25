
#include "../includes/Server.hpp"
#include "../includes/webserv.hpp"
#include <map>

bool g_end;

int main(int argc, char *argv[]) {
	g_end = false;
	init_signals();
	std::vector<config> confs;
	std::map<int, Server> map_serv;
	if (argc == 1) {
		//	confs.reserve(1);
		fill_servers_configs(confs, default_conf);
	} else if (argc == 2) {
		//	confs.reserve(how_many_serv(argv[1]));
		fill_servers_configs(confs, argv[1]);
	}
	//  print_config(confs);
	std::cout << "init server" << RESET << std::endl;
	// init_server(map_serv, confs);
	if (argc < 1 || argv[0])
		(void)argc;
	// Server serv(confs[0]);
	map_serv[3] = Server(confs[0]);
	for (std::map<int, Server>::iterator it = map_serv.begin(); it != map_serv.end(); it++)
		std::cout << "it firsr = " << it->first << " it second = " << it->second.server_fd << RESET << std::endl;
	// for (std::map<int, Server>::iterator it = map_serv.begin(); it != map_serv.end(); it++)
	// 	std::cout << "it.first = " << it->first << ", it.second = " << it->second.server_fd << RESET << std::endl;
	epoll_loop(map_serv);
}
