
#include "../includes/Server.hpp"
#include "../includes/webserv.hpp"

void init_server(std::map<int, Server> &map_serv, std::vector<config> &confs) {
	for (std::size_t i = 0; i < confs.size(); i++) {
		Server temp(confs[i]);
		if (temp.server_fd != -1) {
			map_serv[temp.server_fd] = temp;
			// std::cout << temp.server_fd << RESET << std::endl;
		}
	}
}
