
#include "../../includes/webserv.hpp"
#include <cstddef>

void file_in_string(std::string &sfile, const char *file)
{
	std::string line;
	std::ifstream ifs(file);
	if (!ifs)
	{
		return ;
	}
	if (!sfile.empty())
		sfile.clear();
	while (std::getline(ifs, line))
	{
		sfile += line;
		sfile += "\n";
	}
}

size_t count_words(const char *line, char c)
{
	size_t l;
	size_t i;

	i = 0;
	l = 0;
	while (line[i])
	{
		while (line[i] == c && line[i])
			i++;
		if (line[i] != c && line[i])
			l++;
		while (line[i] != c && line[i])
			i++;
	}
	return (l);
}

void set_method(config &conf, std::string method)
{
	if (method == "GET")
		conf.allowed_method[GET] = 1;
	else if (method == "POST")
		conf.allowed_method[POST] = 1;
	else if (method == "DELETE")
		conf.allowed_method[DELETE] = 1;
	else if (method == "DEFAULT")
	{
		conf.allowed_method[GET] = 1;
		conf.allowed_method[POST] = 1;
		conf.allowed_method[DELETE] = 1;
	}
	else
		conf.allowed_method[INVALID_METHOD] = 1;
}

void set_method(location &loc, std::string method)
{
	if (method == "GET")
		loc.allowed_method[GET] = 1;
	else if (method == "POST")
		loc.allowed_method[POST] = 1;
	else if (method == "DELETE")
		loc.allowed_method[DELETE] = 1;
	else if (method == "DEFAULT")
	{
		loc.allowed_method[GET] = 1;
		loc.allowed_method[POST] = 1;
		loc.allowed_method[DELETE] = 1;
	}
	else
		loc.allowed_method[INVALID_METHOD] = 1;
}

int check_host(std::string &buff, std::string &host)
{
	std::size_t pos = 0;
	std::size_t offset = 0;

	pos = buff.find("Host: ", offset);
	if (pos == std::string::npos)
		return 1;
	pos += 5;
	offset = pos + 1;
	pos = buff.find(":", offset);
	if (pos == std::string::npos)
		return 1;
	host = buff.substr(offset, pos - offset);
	std::cout << "host = " << host << RESET << std::endl;
	return 0;
}