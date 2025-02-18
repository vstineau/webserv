
#include "../../includes/webserv.hpp"

void	file_in_string(std::string &sfile, char *file)
{
	std::string line;
	std::ifstream ifs(file);
	if (!ifs)
	{
		std::cout << "code mieux fdp\n";
		(NULL);
	}
	while (std::getline(ifs, line))
	{
		sfile += line;
		sfile += "\n";
	}
}

size_t count_words(const char *line, char c)
{
	size_t	l;
	size_t	i;

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

void	set_method(location &loc, std::string method)
{
	if (method == "GET")
		loc.allowed_method[GET] = 1;
	else if (method == "POST")
		loc.allowed_method[POST] = 1;
	else if (method == "DELETE")
		loc.allowed_method[DELETE] = 1;
	else
		loc.allowed_method[INVALID_METHOD] = 1;
}
