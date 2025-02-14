

#include "../includes/webserv.hpp"
#include <string>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>

class Files {
	
	public: 
		Files();
		~Files();
	std::map<std::string, std::string>	mimes;

};
