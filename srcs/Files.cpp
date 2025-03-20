
#include "../includes/Files.hpp"
#include "../includes/webserv.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

FileHandler::FileHandler()
{
	setMime();
	setErrorCodes();
	no_file = true;
}

FileHandler::~FileHandler()
{
}

void FileHandler::setFileInfo(std::string path)
{
	struct stat sb;
	if (path.empty())
		return;
	if (stat(path.c_str(), &sb) == -1)
	{
		no_file = true;
		return;
	}
	no_file = false;
	// std::cout << "Type de fichier :                ";
	switch (sb.st_mode & S_IFMT)
	{
	case S_IFDIR:
		type = DIRECTORY;
		break;
	case S_IFREG:
		type = CLASSIC_FILE;
		break;
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
		return;
	}
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
	pre_env.push_back("PATH=/usr/bin:/bin:/usr/local/bin");
	switch (req.method)
	{
	case GET:
		pre_env[3].append("GET", 3);
		break;
	case POST:
		pre_env[3].append("POST", 4);
		break;
	case DELETE:
		pre_env[3].append("DELETE", 7);
		break;
	case INVALID_METHOD:
		pre_env[3].append("NO", 2);
		break;
	}
	envp = (char **)std::calloc(pre_env.size() + 1, sizeof(char *));
	for (std::size_t i = 0; i < pre_env.size(); i++)
		envp[i] = strdup(pre_env[i].c_str());
	return envp;
}

static void close_pipe(int pipe[2])
{
	if (pipe[0] != -1)
		close(pipe[0]);
	if (pipe[1] != -1)
		close(pipe[1]);
}


int FileHandler::execCgi(request &req, location &loc, response &r)
{
	time_t start = time(NULL);
	(void)start;
	int pipe_out[2] = {-1, -1};
	if (pipe(pipe_out))
		return 500;
	int cgi_pid = fork();
	if (cgi_pid == -1)
	{
		close_pipe(pipe_out);
		return 500;
	}
	if (!cgi_pid)
	{
		char **envp;
		char **argv;
		for (size_t i = 0; i < grb.size(); i++)
		{
			if (grb[i] != -1)
				close(grb[i]);
		}
		if (dup2(pipe_out[1], STDOUT_FILENO) == -1)
		{
			close_pipe(pipe_out);
			exit(1);
		}
		close_pipe(pipe_out);
		argv = (char **)calloc(3, sizeof(char *));
		if (argv == NULL)
			exit(1);
		argv[0] = strdup(loc.cgi_bin.c_str());
		if(!argv[0])
		{
			free(argv);
			exit(1);
		}
		argv[1] = strdup(req.path.c_str());
		if (!argv[1])
		{
			free(argv[0]);
			free(argv);
			exit(1);
		}
		envp = getCgiEnv(req);
		if (envp == NULL)
		{
			free(argv[0]);
			free(argv[1]);
			free(argv);
			exit(1);
		}
		execve(loc.cgi_bin.c_str(), argv, envp);
		std::cerr << "exec failed" << RESET << std::endl;
		for (int i = 0; envp[i]; i++)
			free(envp[i]);
		free(argv[0]);
		free(argv[1]);
		free(argv);
		free(envp);
		exit(1);
	}
	int status;
	while (waitpid(-1, &status, WNOHANG) == 0)
	{
		if (time(NULL) - start > 5)
		{
			r.cgi_rep.clear();
			std::cerr << "TIMEOUT" << RESET << std::endl;
			close_pipe(pipe_out);
			kill(cgi_pid, SIGKILL);
			return 504;
		}
	}
	int count = 0;
	char buffer[1025];
	std::string buff;
	close(pipe_out[1]);
	while ((count = read(pipe_out[0], buffer, 1024)) > 0)
		buff.append(buffer, count);
	close_pipe(pipe_out);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0 || count == -1)
	{
		r.cgi_rep.clear();
		return 500;
	}
	r.cgi_rep = buff;
	return 200;
}

void FileHandler::setErrorCodes(void)
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

void FileHandler::setMime(void)
{
	mimes["NO EXTENTION"] = "text/plain";
	mimes["jpg"] = "image/jpeg";
	mimes["jpeg"] = "image/jpeg";
	mimes["gif"] = "image/gif";
	mimes["html"] = "text/html";
	mimes["htm"] = "text/html";
	mimes["css"] = "text/css";
	mimes["json"] = "application/json";
	mimes["js"] = "application/javascript";
	mimes["pdf"] = "application/pdf";
	mimes["png"] = "image/png";
	mimes["zip"] = "application/zip";
	mimes["rar"] = "application/x-rar-compressed";
	mimes["sh"] = "application/x-sh";
	mimes["xml"] = "application/xml";
	mimes["csv"] = "text/csv";
	mimes["txt"] = "text/plain";
	mimes["mp3"] = "audio/mpeg";
	mimes["wav"] = "audio/wav";
	mimes["ogg"] = "audio/ogg";
	mimes["mp4"] = "video/mp4";
	mimes["mov"] = "video/quicktime";
	mimes["avi"] = "video/x-msvideo";
	mimes["mkv"] = "video/x-matroska";
	mimes["webm"] = "video/webm";
	mimes["flv"] = "video/x-flv";
	mimes["doc"] = "application/msword";
	mimes["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	mimes["ppt"] = "application/vnd.ms-powerpoint";
	mimes["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	mimes["xls"] = "application/vnd.ms-excel";
	mimes["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	mimes["odt"] = "application/vnd.oasis.opendocument.text";
	mimes["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	mimes["rtf"] = "application/rtf";
	mimes["epub"] = "application/epub+zip";
	mimes["apk"] = "application/vnd.android.package-archive";
	mimes["exe"] = "application/x-msdownload";
	mimes["json"] = "application/json";
	mimes["svg"] = "image/svg+xml";
	mimes["ico"] = "image/x-icon";
	mimes["tiff"] = "image/tiff";
	mimes["bmp"] = "image/bmp";
	mimes["webp"] = "image/webp";
	mimes["7z"] = "application/x-7z-compressed";
	mimes["tar"] = "application/x-tar";
	mimes["gz"] = "application/gzip";
	mimes["bz2"] = "application/x-bzip2";
	mimes["xz"] = "application/x-xz";
	mimes["msi"] = "application/x-msi";
	mimes["flac"] = "audio/flac";
	mimes["aiff"] = "audio/aiff";
	mimes["amr"] = "audio/amr";
	mimes["aac"] = "audio/aac";
	mimes["midi"] = "audio/midi";
	mimes["wma"] = "audio/x-ms-wma";
	mimes["cue"] = "application/x-cue";
}
