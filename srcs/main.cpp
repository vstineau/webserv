
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
	if (argc < 1 || argv[0])
		 ;
	Server serv;

	serv.setServer_fd();
	epollinit(serv);

}
