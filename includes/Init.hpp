#pragma once

#include "Server.hpp"
#include "webserv.hpp"
#include <arpa/inet.h>
#include <fstream>
#include <map>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

class Init
{

	public:
		Init();
		~Init();
		class BadConfigFileExeption : public std::exception
		{
			public :
				virtual const char *what() const throw()
				{
					return "Error: invalid configuration file\n";
				}
		};
		void fill_servers_configs(char *file);
		void epoll_loop();
		void init_servers();
		void set_default_conf();
		std::vector<config> confs;
		std::vector<Server> servs;
};
