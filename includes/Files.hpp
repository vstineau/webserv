

#include "../includes/webserv.hpp"
#include <string>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>

enum File_type {
	CLASSIC_FILE,
	DIRECTORY
};

class FileHandler {
	
	public: 
		FileHandler();
		~FileHandler();
		void setFileInfo(std::string path);
		void setFile(std::string path);
		void setMime();
		std::map<std::string, std::string>	mimes;
//		getContentType();
//		execCgi();
		long long		file_size;
		std::string		filestring;
		std::string		path;
		File_type		type;
		std::string		extention;
		bool			no_file;
		std::string		Content_Type;
};

//
//POST /images/dumb HTTP/1.1
//Host: hostname
//Connection: close
//Transfer-encoding: chunked
//
//15
//Hi this is chunked !!
//4
//fewf
//6
//wefwef
//3
//wef
//3
//wef
//0
