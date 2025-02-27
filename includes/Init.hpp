#pragma once


#include "webserv.hpp"
#include "Server.hpp"
#include <arpa/inet.h>
#include <map>
#include <string>
#include <fstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

class Init {

	public:
		Init();
		~Init();
		void fill_servers_configs(const std::string &file);
		void epoll_loop();
		void init_servers();
		std::vector<config> confs;
		std::vector<Server> servs;
};
