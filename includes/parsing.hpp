#pragma once

#include "webserv.hpp"

const char default_conf[] = "srcs/default.conf";

template <typename V, typename F>
void fill_servers_configs(V &confs, F *file)
{
	std::ifstream	ifs(file);
	std::string		buffer;
	std::string		temp;

	for(size_t serv_index = 0; serv_index < confs.capacity(); serv_index++)
	{
		std::getline(ifs, buffer);
		while (buffer.empty() && buffer != "Server {")
			std::getline(ifs, buffer);
		//std::cout << BHI_RED << buffer << RESET << std::endl;
		if (buffer == "Server {")
		{
			buffer.clear();
			std::getline(ifs, temp);
			//std::cout << BHI_GREEN << temp << RESET << std::endl;
			while (temp != "}")
			{
				std::getline(ifs, temp);
				buffer += temp;
			}
		}
		std::cout << "_______CONF N." << serv_index << "________" << std::endl;
		std::cout << buffer << std::endl;
		std::cout << "_______________________" << std::endl;
		std::cout << std::endl;
		get_one_config(confs[serv_index], buffer);
	}
}
