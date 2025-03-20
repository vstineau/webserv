
#include "../includes/Server.hpp"
#include "../includes/webserv.hpp"

std::string get_body_error(int status_code)
{
	size_t pos;
	size_t offset = 0;
	std::string errbody = error_body;
	std::string code = to_string(status_code);
	if (!code.empty())
	{
		pos = errbody.find("%{code_error}%", offset);
		while (pos != std::string::npos)
		{
			errbody.erase(pos, 14);
			errbody.insert(pos, code);
			offset = pos + code.length();
			pos = errbody.find("%{code_error}%", offset);
		}
	}
	return (errbody);
}
