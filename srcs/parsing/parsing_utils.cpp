
#include "../../includes/webserv.hpp"

size_t count_words(char *line, char c)
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

