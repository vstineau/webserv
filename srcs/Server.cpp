
#include "../includes/Server.hpp"
#include "../includes/color.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Server::~Server()
{
	for (std::vector<int>::iterator it = client_fd.begin(); it != client_fd.end(); it++)
		if (*it != -1)
			close(*it);
	if (server_fd != -1)
		close(server_fd);
}

// Server::Server() : server_fd(-1), address(), status_code(200)
Server::Server() : server_fd(-1), address(), status_code(200)
{
	setErrorCodes();
}

// Server::Server(config &conf) : server_fd(-1), status_code(200), _conf(conf)
Server::Server(config &conf) : server_fd(-1), status_code(200), _conf(conf)
{
	setErrorCodes();
	// _conf.locations["www/"] = location;
}

void Server::setSocket()
{
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_conf.port);
	address = addr;

	int b = true;
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		perror("socket");
	}
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &b, sizeof(int));
	if (bind(server_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
	{
		perror("bind");
		close(server_fd);
	}
	if (listen(server_fd, 10) == -1)
	{
		perror("listen");
		close(server_fd);
	}
}

// return response in one string ready to get send to the client
std::string Server::getResponse(void) const
{
	std::string r;
	r += _response.status_line;
	r += "\r\n";
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = _response.headers.begin();
		 it != _response.headers.end(); it++)
	{
		for (size_t i = 0; i < it->second.size(); i++)
		{
			r += it->first;
			r += it->second[i];
			r += "\r\n";
		}
	}
	r += "\r\n";
	r += _response.body;
	return (r);
}

void Server::setErrorCodes(void)
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

void Server::SetErrorResponse(int error_code)
{
	status_code = error_code;
	SetResponseStatus(status_code);
	_response.body = get_body_error(error_code);
	_response.headers["Content-Type: "].push_back("text/html");
	_response.headers["Content-Length: "].push_back(to_string(_response.body.length()));
}

int Server::checkLocations(request &req)
{
	if (req.path == _conf.root || req.path == _conf.root + "/")
	{
		if (!_conf.allowed_method[GET])
		{
			SetErrorResponse(405);
			return 1;
		}
		status_code = 200;
		SetResponseStatus(status_code);
		std::string path = req.path + "/index.html";
		file_in_string(_response.body, path.c_str());
		_response.headers["Content-Type: "].push_back("text/html");
		_response.headers["Content-Length: "].push_back(to_string(_response.body.length()));
		return 1;
	}
	for (std::map<std::string, location>::iterator it = _conf.locations.begin(); it != _conf.locations.end(); it++)
	{
		if (it->first == req.path.substr(_conf.root.length()))
		{
			if (!it->second.allowed_method[GET])
			{
				SetErrorResponse(405);
				return 1;
			}
			status_code = 200;
			SetResponseStatus(status_code);
			std::string path = req.path + "/index.html";
			file_in_string(_response.body, path.c_str());
			_response.headers["Content-Type: "].push_back("text/html");
			_response.headers["Content-Length: "].push_back(to_string(_response.body.length()));
			return 1;
		}
	}
	return 0;
}

void Server::_responseGET(request &req)
{
	if (checkLocations(req))
		return;
	_file.setFileInfo(req.path);
	_file.setFile(req.path);
	if (_file.extention != "NO EXTENTION")
	{
		std::ifstream imgFile(req.path.c_str());
		if (!imgFile)
		{
			SetErrorResponse(404);
			return;
		}
		else
		{
			SetResponseStatus(status_code);
			std::cout << "file was opened\n";
			_response.body = _file.filestring;
			SetResponseStatus(status_code);
			_response.headers["Content-Type: "].push_back(_file.mimes[_file.extention]);
			_response.headers["Content-Length: "].push_back(to_string(_response.body.length()));
		}
	}
	else
	{
		SetErrorResponse(404);
		return;
	}
	return;
}

std::string Server::checkUpload(request &req)
{
	if (req.path == _conf.root)
	{
		if (_conf.allowed_method[POST])
		{
			if ((int)_response.body.size() < _conf.client_body_size)
			{
				if (!_conf.upload_directory.empty())
					return _conf.root + "/" + _conf.upload_directory;
				else
					return _conf.root;
			}
			else
			{
				SetErrorResponse(413);
				return "";
			}
		}
		else
		{
			SetErrorResponse(405);
			return "";
		}
	}
	else
	{
		for (std::map<std::string, location>::iterator it = _conf.locations.begin(); it != _conf.locations.end(); it++)
		{
			if (req.path.substr(_conf.root.size()) == it->first)
			{
				if (it->second.allowed_method[POST])
				{
					if ((int)req.body.size() < it->second.client_body_size)
					{
						if (!it->second.upload_directory.empty())
							return _conf.root + it->second.root + it->second.upload_directory;
						else
							return _conf.root + it->second.root;
					}
					else
					{
						SetErrorResponse(413);
						return "";
					}
				}
				else
				{
					SetErrorResponse(405);
					return "";
				}
			}
		}
		SetErrorResponse(404);
		return "";
	}
}

void Server::_responsePOST(request &req, int &n)
{
	(void)req;
	std::string up_dir = checkUpload(req);
	if (up_dir.empty())
		return;
	std::cout << "up_dir = " << up_dir << std::endl;
	if (fill_body(req.body, n, up_dir))
	{
		SetErrorResponse(403);
		return;
	}
	std::string path = _conf.root + "index.html";
	file_in_string(_response.body, path.c_str());
	SetResponseStatus(status_code);
	_response.headers["Content-Type: "].push_back("text/html");
	_response.headers["Content-Length: "].push_back(to_string(_response.body.length()));
	return;
}

void Server::_DELETEmethod(request &req)
{
	if (unlink(req.path.c_str()) == -1)
	{
		status_code = 404;
		SetResponseStatus(status_code);
		_response.body = get_body_error(404);
	}
	std::string path = _conf.root + "index.html";
	file_in_string(_response.body, path.c_str());
	SetResponseStatus(status_code);
	_response.headers["Content-Type: "].push_back("text/html");
	_response.headers["Content-Length: "].push_back(to_string(_response.body.length()));
	return;
}

void Server::_responseDELETE(request &req)
{
	if (req.path == _conf.root)
	{
		if (!_conf.allowed_method[DELETE])
		{
			SetErrorResponse(405);
			return ;
		}
		else
			_DELETEmethod(req);// faire la methode DELETE
	}
	else
	{
		for (std::map<std::string, location>::iterator it = _conf.locations.begin(); it != _conf.locations.end(); it++)
		{
			if(req.path == it->first)
			{
				if (!it->second.allowed_method[DELETE])
				{
					SetErrorResponse(405);
					return ;
				}
				else
					_DELETEmethod(req);// faire la methode DELETE
			}
		}
	}
	SetErrorResponse(404);
}

void Server::SetResponseStatus(int n)
{
	_response.status_line = "HTTP/1.1 ";
	_response.status_line += to_string(n);
	_response.status_line += _error_codes[n];
}

void Server::clear_response()
{
	_response.status_line.clear();
	_response.headers.clear();
	_response.body.clear();
}

void Server::SetResponse(int n)
{
	if (_requests[n].method == GET)
		_responseGET(_requests[n]);
	else if (_requests[n].method == POST)
		_responsePOST(_requests[n], n);
	else if (_requests[n].method == DELETE)
		_responseDELETE(_requests[n]);
}

int Server::create_img(std::string &img, std::string &up_dir)
{
	(void)up_dir;
	size_t pos = 0;
	size_t offset = 0;
	std::string filename;
	std::string content;

	pos = img.find("filename=\"", offset);
	std::cout << "HERE" << RESET << std::endl;
	if (pos == std::string::npos)
		return 1;
	offset = pos + 10;
	pos = img.find("\"", offset);
	filename = img.substr(offset, pos - offset);
	std::cout << filename << RESET << std::endl;
	offset = pos + 1;
	std::string path = up_dir + filename;
	std::ofstream ofs(path.c_str(), std::ios_base::binary);
	if (!ofs.is_open())
		return 1;
	offset = pos + 1;
	pos = img.find("\r\n\r\n", offset);
	if (pos == std::string::npos)
		return 1;
	offset = pos + 4;
	content = img.substr(offset, img.size() - offset);
	ofs << content;
	return 0;
}

int Server::fill_body(std::string &body, int &n, std::string &up_dir)
{
	(void)up_dir;
	size_t pos = 0;
	size_t offset = 0;
	std::string img;
	while (pos != std::string::npos)
	{
		pos = body.find("\n", offset);
		if (pos == std::string::npos)
			return 0;
		offset = pos + 1;
		pos = body.find(_requests[n].headers["boundary"], offset);
		if (pos == std::string::npos)
			return 0;
		img = body.substr(offset, pos - offset);
		if (create_img(img, up_dir))
			return 1;
		offset = pos + _requests[n].headers["boundary"].size();
	}
	return 0;
}

void Server::fill_header(std::string &header, int &n)
{
	size_t pos = 0;
	size_t offset = 0;
	std::string key;
	while (pos != std::string::npos)
	{
		pos = header.find(":", offset);
		if (pos == std::string::npos)
		{
			return;
		}
		key = header.substr(offset, pos - offset);
		offset = pos + 1;
		if (key == "Content-Type")
		{
			pos = check_contentype(n, pos, offset, header);
			if (pos == std::string::npos)
			{
				break;
			}
		}
		else
		{
			pos = header.find("\n", offset);
			if (pos == std::string::npos)
			{
				break;
			}
			_requests[n].headers[key] = header.substr(offset, pos - offset);
		}
		offset = pos + 1;
	}
	//	if (!_requests[n].headers["Cookie"].empty())
	//		fill_cookie(_requests[n].headers["Cookie"]);
}

std::size_t Server::check_contentype(int n, std::size_t pos, std::size_t offset, std::string &buffer)
{
	std::string tmp;
	std::string key1("Content-Type");
	std::string key2("boundary");
	std::size_t posendline;

	posendline = buffer.find("\r\n", offset);
	if (posendline == std::string::npos)
		return (posendline);
	tmp = buffer.substr(offset, pos - offset);
	pos = buffer.find(key2, offset);
	if (pos == std::string::npos)
	{
		_requests[n].headers[key1] = buffer.substr(offset, posendline - offset);
		return (posendline);
	}
	_requests[n].headers[key1] = buffer.substr(offset, pos - offset);
	offset = pos + 10;
	_requests[n].headers[key2] = buffer.substr(offset, posendline - offset);
	return (posendline);
}

void Server::clear_request(int n)
{
	if (!_requests[n].body.empty())
		_requests[n].body.clear();
}

void Server::fill_query(int n)
{
	size_t pos = 0;

	pos = _requests[n].path.find("?");
	if (pos == std::string::npos)
		return;
	pos += 1;
	_requests[n].query = _requests[n].path.substr(pos, _requests[n].path.size() - pos);
	_requests[n].path.erase(pos - 1, _requests[n].path.size() - (pos - 1));
}

void Server::fillRequest(int n, std::string &buffer)
{
	size_t pos = 0;
	size_t offset = 0;
	std::string header;
	std::string body;

	clear_request(n);
	if (!buffer.find("GET"))
	{
		_requests[n].method = GET;
		offset = pos + 3;
	}
	else if (!buffer.find("POST"))
	{
		_requests[n].method = POST;
		offset = pos + 4;
	}
	else if (!buffer.find("DELETE"))
	{
		_requests[n].method = DELETE;
		offset = pos + 6;
	}
	pos = buffer.find("/", offset);
	if (pos == std::string::npos)
	{
		return;
	}
	offset = pos;
	pos = buffer.find(" ", offset);
	if (pos == std::string::npos)
	{
		return;
	}
	_requests[n].path = buffer.substr(offset, pos - offset);
	fill_query(n);
	_requests[n].path.replace(0, 1, _conf.root);
	offset = pos + 1;
	pos = buffer.find("\n", offset);
	if (pos == std::string::npos)
	{
		return;
	}
	_requests[n].version = buffer.substr(offset, pos - offset);
	offset = pos + 1;
	pos = buffer.find("\r\n\r\n", offset);
	if (pos == std::string::npos)
	{
		return;
	}
	header = buffer.substr(offset, pos - offset);
	fill_header(header, n);
	offset = pos + 4;
	_requests[n].body = buffer.substr(offset, buffer.size() - offset);
}
