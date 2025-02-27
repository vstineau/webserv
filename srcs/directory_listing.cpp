
#include "../includes/webserv.hpp"
#include <dirent.h>
#include <string.h>
#include <string>
#include <sys/types.h>

std::string get_directory_line(std::string thing)
{
	std::string line("      <p><a href=\"http://127.0.0.1:8080/%{img}%\" >%{img}%</a></p>\n");
	size_t pos;
	size_t offset = 0;
	pos = line.find("%{img}%", offset);
	while (pos != std::string::npos)
	{
		line.erase(pos, 7);
		line.insert(pos, thing);
		offset = pos + thing.length();
		pos = line.find("%{img}%", offset);
	}
	return (line);
}

std::string directory_listing(std::string path)
{
	std::string list;
	DIR *dir = opendir(path.c_str());

	if (dir == NULL)
		perror("opendir failed");
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, "."))
			list += get_directory_line(entry->d_name);
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
		if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
			list += get_directory_line(entry->d_name);
	}
	if (closedir(dir) == -1)
		perror("closerdir failed");
	return list;
}
