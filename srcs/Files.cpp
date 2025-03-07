
#include "../includes/webserv.hpp"
#include <map>
#include "../includes/Files.hpp"
#include <fstream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

FileHandler::FileHandler()
{
	setMime();
	setErrorCodes();
	no_file = true;
}

FileHandler::~FileHandler()
{}

void FileHandler::setFileInfo(std::string path)
{
	struct stat sb;
	if (path.empty())
		return ;
	if (stat(path.c_str(), &sb) == -1)
	{
		no_file = true;
		return ;
	}
	no_file = false;
	std::cout << "Type de fichier :                ";
	switch (sb.st_mode & S_IFMT)
	{
		case S_IFDIR:  type = DIRECTORY; break;
		case S_IFREG:  type = CLASSIC_FILE; break;
	}
	file_size = (long long)sb.st_size;
}

void FileHandler::setFile(std::string path)
{
	std::string line;
	if (!filestring.empty())
		filestring.clear();
	std::ifstream ifs(path.c_str());
	if (!ifs)
	{
		(NULL);
	}
	while (std::getline(ifs, line))
	{
		filestring += line;
		filestring += "\n";
	}
	std::size_t pos;
	pos = path.find(".");
	if (pos == std::string::npos)
	{
		extention = "NO EXTENTION";
		return ;
	}
	pos += 1;
	extention = path.substr(pos, path.size() - pos);
	Content_Type = mimes[extention];
}

std::string FileHandler::getCgiStatusLine(int code)
{
	std::string status_line("HTTP/1.1 ");
	status_line += to_string(code);
	status_line += _error_codes[code];
	return (status_line);
}

char **FileHandler::getCgiEnv(request &req)
{
	char **envp;
	std::vector<std::string> pre_env;
	pre_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	pre_env.push_back("SCRIPT_FILENAME=");
	pre_env[1].append(req.path, req.path.size());
	pre_env.push_back("QUERY_STRING=");
	pre_env[2].append(req.query, req.query.size());
	pre_env.push_back("REQUEST_METHOD=");
	switch (req.method)
	{
		case GET:
			pre_env[3].append("GET", 3); break;
		case POST:
			pre_env[3].append("POST", 4); break;
		case DELETE:
			pre_env[3].append("DELETE", 7); break;
		case INVALID_METHOD:
			pre_env[3].append("NO", 2); break;
	}
	envp = (char **)std::calloc(pre_env.size() + 1, sizeof(char *));
	for (std::size_t i = 0; i < pre_env.size(); i++)
		envp[i] = strdup(pre_env[i].c_str());
	return envp;
}

// response FileHandler::execCgi(request &req, std::string &bin_path, std::string &extention)
// {
// 	//rajouter un truc pour detecter si c'est un cgi 
// 	response r;
// 	std::string cmd;
//
// 	cmd = bin_path + " " + req.path;
// 	time_t start = time(NULL);
// 	int  cgi_pid = fork();
// 	if (cgi_pid == -1)
// 		;//500
// 	if (!cgi_pid)
// 	{
//		char **envp;
//		char **argv;
//
//		argv = (char **)calloc(2, sizeof(char *));
//		if (argv == NULL)
//			;//500
//		argv[0] = strdup(cmd.c_str());
//		if (argv[0] == NULL)
//			;//500
//		argv[1] = NULL;
//		envp = getCgiEnv(req);
//		if (envp == NULL)
//			;//500
//		execve(*argv, argv, envp);
//		//faire un free pour **envp
// 	//error 500;
// 	}
// 	int status;
// 	while (waitpid(cgi_pid, &status,WNOHANG) > 0)
// 	{
// 		time_t timeout = time(NULL);
// 		if (timeout - start < 5)
// 		{
// 			kill(cgi_pid, SIGKILL);
// 			//timeout jsp pas on fais quoi mais on le fais
// 		}
//
// 	}
//
// 	/*
// 	meta-variable-name = "AUTH_TYPE" | "CONTENT_LENGTH" |
// 	"CONTENT_TYPE" | "GATEWAY_INTERFACE" |
// 	"PATH_INFO" | "PATH_TRANSLATED" |
// 	"QUERY_STRING" | "REMOTE_ADDR" |
// 	"REMOTE_HOST" | "REMOTE_IDENT" |
// 	"REMOTE_USER" | "REQUEST_METHOD" |
// 	"SCRIPT_NAME" | "SERVER_NAME" |
// 	"SERVER_PORT" | "SERVER_PROTOCOL" |
// 	"SERVER_SOFTWARE" | scheme |
// 	protocol-var-name | extension-var-name	*/
// 	return r;
// }
//
void	FileHandler::setErrorCodes(void)
{
	_error_codes[100] = " Continue";
	_error_codes[101] = " Switching Protocols";
	_error_codes[200] = " OK";
	_error_codes[201] = " Created";
	_error_codes[202] = " Accepted";
	_error_codes[203] = " Non-Authoritative Information";
	_error_codes[204] = " No Content";
	_error_codes[205] = " Reset Content";
	_error_codes[206] = " Partial Content";
	_error_codes[300] = " Multiple Choices";
	_error_codes[301] = " Moved Permanently";
	_error_codes[302] = " Found";
	_error_codes[303] = " See Other";
	_error_codes[304] = " Not Modified";
	_error_codes[305] = " Use Proxy";
	_error_codes[307] = " Temporary Redirect";
	_error_codes[400] = " Bad Request";
	_error_codes[401] = " Unauthorized";
	_error_codes[402] = " Payment Required";
	_error_codes[403] = " Forbidden";
	_error_codes[404] = " Not Found";
	_error_codes[405] = " Method Not Allowed";
	_error_codes[406] = " Not Acceptable";
	_error_codes[407] = " Proxy Authentication Required";
	_error_codes[408] = " Request Time-out";
	_error_codes[409] = " Conflict";
	_error_codes[410] = " Gone";
	_error_codes[411] = " Length Required";
	_error_codes[412] = " Precondition Failed";
	_error_codes[413] = " Request Entity Too Large";
	_error_codes[414] = " Request-URI Too Large";
	_error_codes[415] = " Unsupported Media Type";
	_error_codes[416] = " Requested range not satisfiable";
	_error_codes[417] = " Expectation Failed";
	_error_codes[500] = " Internal Server Error";
	_error_codes[501] = " Not Implemented";
	_error_codes[502] = " Bad Gateway";
	_error_codes[503] = " Service Unavailable";
	_error_codes[504] = " Gateway Time-out";
	_error_codes[505] = " HTTP Version not supported";
}

void	FileHandler::setMime(void)
{
	mimes["NO EXTENTION"] = "text/plain";
	mimes["jpg"]     = "image/jpeg";
	mimes["jpeg"]    = "image/jpeg";
	mimes["gif"]     = "image/gif";
	mimes["html"]    = "text/html";
	mimes["htm"]     = "text/html";
	mimes["css"]     = "text/css";
	mimes["json"]    = "application/json";
	mimes["js"]      = "application/javascript";
	mimes["pdf"]     = "application/pdf";
	mimes["png"]     = "image/png";
	mimes["zip"]     = "application/zip";
	mimes["rar"]     = "application/x-rar-compressed";
	mimes["sh"]      = "application/x-sh";
	mimes["xml"]     = "application/xml";
	mimes["csv"]     = "text/csv";
	mimes["txt"]     = "text/plain";
	mimes["mp3"]     = "audio/mpeg";
	mimes["wav"]     = "audio/wav";
	mimes["ogg"]     = "audio/ogg";
	mimes["mp4"]     = "video/mp4";
	mimes["mov"]     = "video/quicktime";
	mimes["avi"]     = "video/x-msvideo";
	mimes["mkv"]     = "video/x-matroska";
	mimes["webm"]    = "video/webm";
	mimes["flv"]     = "video/x-flv";
	mimes["doc"]     = "application/msword";
	mimes["docx"]    = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	mimes["ppt"]     = "application/vnd.ms-powerpoint";
	mimes["pptx"]    = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	mimes["xls"]     = "application/vnd.ms-excel";
	mimes["xlsx"]    = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	mimes["odt"]     = "application/vnd.oasis.opendocument.text";
	mimes["ods"]     = "application/vnd.oasis.opendocument.spreadsheet";
	mimes["rtf"]     = "application/rtf";
	mimes["epub"]    = "application/epub+zip";
	mimes["apk"]     = "application/vnd.android.package-archive";
	mimes["exe"]     = "application/x-msdownload";
	mimes["json"]    = "application/json";
	mimes["svg"]     = "image/svg+xml";
	mimes["ico"]     = "image/x-icon";
	mimes["tiff"]    = "image/tiff";
	mimes["bmp"]     = "image/bmp";
	mimes["webp"]    = "image/webp";
	mimes["7z"]      = "application/x-7z-compressed";
	mimes["tar"]     = "application/x-tar";
	mimes["gz"]      = "application/gzip";
	mimes["bz2"]     = "application/x-bzip2";
	mimes["xz"]      = "application/x-xz";
	mimes["msi"]     = "application/x-msi";
	mimes["flac"]    = "audio/flac";
	mimes["aiff"]    = "audio/aiff";
	mimes["amr"]     = "audio/amr";
	mimes["aac"]     = "audio/aac";
	mimes["midi"]    = "audio/midi";
	mimes["wma"]     = "audio/x-ms-wma";
	mimes["cue"]     = "application/x-cue";
}
