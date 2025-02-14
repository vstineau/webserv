
#include "../includes/webserv.hpp"
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>


std::string directory_listing(std::string path)
{
	std::string list;
	DIR *dir = opendir(path.c_str());

	if (dir == NULL)
		perror("opendir failed");
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		if(strcmp(entry->d_name, "."))
		{
			list += entry->d_name;
			list += "\n";
		}
	}
	if (closedir(dir) == -1)
		perror("closerdir failed");
	return list;
}

std::string upload(std::string path)
{
	std::string list;
	DIR *dir = opendir(path.c_str());

	if (dir == NULL)
		perror("opendir failed");
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
		{
			list += entry->d_name;
			list += "\n";
		}
	}
	if (closedir(dir) == -1)
		perror("closerdir failed");
	return list;
}
