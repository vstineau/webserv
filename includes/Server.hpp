
#pragma once
// #ifndef SERVER_HPP
// #define SERVER_HPP

#include "../includes/Files.hpp"
#include "../includes/webserv.hpp"
#include <arpa/inet.h>
#include <map>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

//%{code_error}%
static const std::string error_body =
	"<!DOCTYPE html>" ENDL "<html lang=\"en\"> " ENDL "<head>" ENDL "	<meta charset=\"UTF-8\">" ENDL
	"	<meta name=\"error\" content=\"width=device-width, initial-scale=1.0\">" ENDL "		<title>%{code_error}%</title>" ENDL
	"		<style> body{	background: content-box radial-gradient(#0664dc, #33b203);" ENDL
	"									font-family: cursive;" ENDL "									font-size: 3.2rem;" ENDL
	"									display: flex;" ENDL "									width: 100%;" ENDL
	"								height: 100%;" ENDL "								justify-content: center;" ENDL
	"								}" ENDL "		</style>" ENDL "</head>" ENDL "<body>" ENDL
	"		<img src=https://http.cat/%{code_error}%.jpg /> " ENDL "</body>" ENDL;

class Server
{

	public:
	Server();
	Server(config &conf);
	~Server();

	void setErrorCodes(void);
	void add_root(void);
	void setSocket();
	void SetResponse(int n);
	std::string getResponse(void) const;
	void SetResponseStatus(int n);
	void print_response(response &rep);
	void print_request(int n);
	void clear_request(int n);
	void clear_response();
	void fillRequest(int n, std::string &buffer);
	int server_fd;
	std::vector<int> client_fd;
	struct sockaddr_in address;
	int status_code;

	private:
	void fill_header(std::string &header, int &n);
	void fill_query(int n);
	void fill_cookie(std::string &header);
	int fill_body(std::string &header, int &n, std::string &up_dir);
	int create_img(std::string &img, std::string &up_dir);
	std::size_t check_contentype(int n, std::size_t pos, std::size_t offset, std::string &buffer);
	void _responseGET(request &rep);
	void _responsePOST(request &rep, int &n);
	std::string checkUpload(request &req);
	void _responseDELETE(request &rep);
	void _DELETEmethod(request &req);
	int checkLocations(request &req);
	void SetErrorResponse(int error_code);
	config _conf;
	std::map<int, request> _requests;
	std::map<int, std::string> _error_codes;
	response _response;
	FileHandler _file;
};

// #endif
