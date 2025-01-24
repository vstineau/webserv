
#include "../includes/webserv.hpp"
#include "../includes/Server.hpp"

int main(int argc, char *argv[])
{
//	if (configuration_file_error(argc, argv[1]))
//		return (1);
//		try
//		{
//			//djfsdjfdf
//		}
//		catch (std::exception &e)
//		{
//			std::cout <<  e.what();
//		}
//	}
	std::vector<config> confs;
	if (argc == 1)
	{
		confs.reserve(1);
		fill_servers_configs(confs, default_conf);
	}
	else if (argc == 2)
	{
		confs.reserve(how_many_serv(argv[1]));
		fill_servers_configs(confs, argv[1]);
	}
//	if (argc < 1 || argv[0])
//		 ;
//	Server serv;
//
//	serv.setServer_fd();
//	epollinit(serv);

}
