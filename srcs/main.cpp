
#include "../includes/webserv.hpp"

int configuration_file_error(int argc, char *file)
{
	std::ifstream ifs(file);
	if (argc < 2)
		return (std::cout << "Error: missing configuration file\n", 1);
	if (argc > 2)
		return (std::cout << "Error: too many argument, only one configuration file is needed\n", 1);
	else if (!ifs)
		return (std::cout << "Error: can't open configuration file\n", 1);
	return (0);
}

int main(int argc, char *argv[])
{
	if (configuration_file_error(argc, argv[1]))
		return (1);
	while (2023)
	{}
}
