
#include "../includes/webserv.hpp"
#include "../includes/Server.hpp"

bool	g_end;

int main(int argc, char *argv[])
{
	g_end = false;
	init_signals();
//	std::vector<config> confs;
//	std::vector<Server> servs;
//	if (argc == 1)
//	{
//		confs.reserve(1);
//		fill_servers_configs(confs, default_conf);
//	}
//	else if (argc == 2)
//	{
//		confs.reserve(how_many_serv(argv[1]));
//		fill_servers_configs(confs, argv[1]);
//	}
	if (argc < 1 || argv[0])
		(void)argc;
	Server serv;

	serv.setServer_fd();
	epollinit(serv);
}
