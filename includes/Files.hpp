

#include "../includes/webserv.hpp"
#include <arpa/inet.h>
#include <map>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

enum File_type
{
	CLASSIC_FILE,
	DIRECTORY
};

class FileHandler
{

	public:
	FileHandler();
	~FileHandler();
	void setFileInfo(std::string path);
	void setFile(std::string path);
	void setMime();
	std::map<std::string, std::string> mimes;
	int execCgi(request &req, location &loc, response &r);
	char **getCgiEnv(request &req);
	long long file_size;
	std::string filestring;
	std::string path;
	File_type type;
	std::string extention;
	bool no_file;
	std::string Content_Type;
};
