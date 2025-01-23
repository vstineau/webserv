
#include "../includes/webserv.hpp"
#include "../includes/Server.hpp"

//int configuration_file_error(int argc, char *file)
//{
//	std::ifstream ifs(file);
//	if (argc < 2)
//		return (std::cerr << "Error: missing configuration file\n", 1); //server has to run even without configuration file
//	if (argc > 2)
//		return (std::cerr << "Error: too many argument, only one configuration file is needed\n", 1);
//	else if (!ifs)
//		return (std::cerr << "Error: can't open configuration file\n", 1); //so run server like there is no configuration file
//	return (0);
//}
//

size_t	how_many_serv(char *file)
{
	std::string line;
	std::ifstream ifs(file);
	size_t count = 0;
	if (!ifs)
		return (std::cerr << "Error: can't open configuration file\n", 1); //so run server like there is no configuration file
	while (std::getline(ifs, line))
	{
		if (line == "Server {" )
			count ++;
	}
	return (count);
}

void	get_one_config(config &conf, std::string &buffer)
{
	size_t				pos = 0;
	size_t				offset = 0;
}

void fill_servers_configs(std::vector<config> &confs, char *file)
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
	if (argc == 2)
	{
		std::vector<config> confs;
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
