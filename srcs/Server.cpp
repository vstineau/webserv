
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

Server::Server() : server_fd(-1), address(), status_code(200)
{
	setErrorCodes();
}

Server::Server(config &conf) : server_fd(-1), status_code(200), _conf(conf), server_name(conf.server_name)
{
	setErrorCodes();
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
		perror("socket"); // return error
	}
	grb.push_back(server_fd);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &b, sizeof(int));
	if (bind(server_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
	{
		perror("bind"); // return error
		close(server_fd);
	}
	if (listen(server_fd, 10) == -1)
	{
		perror("listen"); // return error
		close(server_fd);
	}
}

// return response in one string ready to get send to the client
std::string Server::getResponse(void) const
{
	if (!_response.cgi_rep.empty())
	{
		std::string cgi_rep(_response.status_line);
		cgi_rep += "\r\n";

		for (std::map<std::string, std::vector<std::string> >::const_iterator it = _response.headers.begin();
			 it != _response.headers.end(); it++)
		{
			for (size_t i = 0; i < it->second.size(); i++)
			{
				cgi_rep += it->first;
				cgi_rep += it->second[i];
				cgi_rep += "\r\n";
			}
		}
		cgi_rep += "\r\n";
		cgi_rep += _response.cgi_rep;
		// std::cout << "cgi rep = " << cgi_rep << RESET << std::endl;
		return cgi_rep;
	}
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
	if (_conf.error_pages[error_code].size() != 0)
	{
		std::string path = _conf.root + SLASH + _conf.error_pages[error_code];
		if (file_in_string(_response.body, path.c_str()))
			_response.body = get_body_error(404);
	}
	else
		_response.body = get_body_error(error_code);
	_response.headers["Content-Type: "].push_back("text/html");
	_response.headers["Content-Length: "].push_back(to_string(_response.body.length()));
}

int Server::checkLocations(request &req, location &loc)
{
	status_code = 200;
	SetResponseStatus(status_code);
	std::string path = req.path + "/index.html";
	if (loc.directory_listing)
	{
		_response.body = directory_listing(req.path);
		fillBodyResponse(_response.body);
	}
	else
	{
		if (file_in_string(_response.body, path.c_str()))
		{
			SetErrorResponse(404);
			return 1;
		}
	}
	_response.headers["Content-Type: "].push_back("text/html");
	_response.headers["Content-Length: "].push_back(to_string(_response.body.length()));
	return 1;
}

int Server::allowedMethod(request &req, location &ret_loc)
{
	std::string path = req.path.substr(0, req.path.rfind("/") + 1);
	std::string file = req.path.substr(req.path.rfind("/") + 1);
	std::string root = _conf.root;
	// std::cout << "req.path = " << req.path << RESET << std::endl;
	for (std::map<std::string, location>::iterator it = _conf.locations.begin(); it != _conf.locations.end(); it++)
	{
		if (it->first.empty())
		{
			if (root + SLASH + file == req.path || root + SLASH + it->second.upload_directory + SLASH + file == req.path)
			{
				if (!it->second.allowed_method[req.method])
					return 405;
				else
				{
					ret_loc = it->second;
					if (it->first != it->second.root)
						return 302;
					return 200; // exec_cgi
				}
			}
			continue;
		}
		if (root + SLASH + it->second.root + SLASH + file == req.path ||
			root + SLASH + it->second.root + SLASH + it->second.upload_directory + SLASH + file == req.path)
		{
			if (!it->second.allowed_method[req.method])
				return 405;
			else
			{
				ret_loc = it->second;
				if (it->first != it->second.root && !it->second.root.empty())
					return 302;
				return 200;
			}
		}
	}
	return 404;
}

int Server::isCGI(request &req, location &loc)
{
	if (_file.extention == loc.cgi_extention)
	{
		int code = _file.execCgi(req, loc, _response);
		// std::cout << "" << RESET << std::endl;
		if (code != 200 && code != 302)
		{
			status_code = code;
			SetErrorResponse(code);
			return 1;
		}
		if(loc.cgi_extention == ".php")
			status_code = 307;
		SetResponseStatus(status_code);
		// _response.headers["Content-Type: "].push_back("text/html");
		// _response.headers["Content-Length: "].push_back(to_string(_response.cgi_rep.size()));
		// _response.cgi_rep.insert(0, _response.status_line.c_str());
		return 1;
	}
	std::cout << "HERE" << RESET << std::endl;
	return 0;
}

void Server::_responseGET(request &req, location &loc)
{
	_file.setFileInfo(req.path);
	_file.setFile(req.path);
	if (isCGI(req, loc))
		return;
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
			_response.body = _file.filestring;
			SetResponseStatus(status_code);
			_response.headers["Content-Type: "].push_back(_file.mimes[_file.extention]);
			_response.headers["Content-Length: "].push_back(to_string(_response.body.length()));
		}
	}
	else if (checkLocations(req, loc))
		return;
	else
	{
		SetErrorResponse(404);
		return;
	}
	return;
}

std::string Server::checkUpload(request &req)
{
	for (std::map<std::string, location>::iterator it = _conf.locations.begin(); it != _conf.locations.end(); it++)
	{
		if (req.path.substr(_conf.root.size() + 1) == it->first)
		{
			if ((unsigned long)req.body.size() < it->second.client_body_size)
			{
				if (it->first.empty())
				{
					if (!it->second.upload_directory.empty())
						return it->second.root + SLASH + it->second.upload_directory + SLASH;
					return it->second.root + SLASH;
				}
				else
				{
					if (!it->second.upload_directory.empty())
						return _conf.root + SLASH + it->second.root + SLASH + it->second.upload_directory + SLASH;
					return _conf.root + SLASH + it->second.root + SLASH;
				}
			}
			else
			{
				SetErrorResponse(413);
				return "";
			}
		}
	}
	SetErrorResponse(404);
	return "";
}

void Server::_responsePOST(request &req, int &n)
{
	(void)req;
	std::string up_dir = checkUpload(req);
	if (up_dir.empty())
		return;
	// std::cout << "up_dir = " << up_dir << std::endl;
	if (fill_body(req.body, n, up_dir))
	{
		SetErrorResponse(403);
		return;
	}
	std::string path = _conf.root + "/index.html";
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
	std::string path = _conf.root + "/index.html";
	file_in_string(_response.body, path.c_str());
	SetResponseStatus(status_code);
	_response.headers["Content-Type: "].push_back("text/html");
	_response.headers["Content-Length: "].push_back(to_string(_response.body.length()));
	return;
}

void Server::_responseDELETE(request &req)
{
	std::string path = req.path.substr(0, req.path.rfind("/") + 1);
	std::string file = req.path.substr(req.path.rfind("/") + 1);
	std::string root = _conf.root;
	for (std::map<std::string, location>::iterator it = _conf.locations.begin(); it != _conf.locations.end(); it++)
	{
		if (it->first.empty())
		{
			if (it->second.root + SLASH + file == req.path ||
				it->second.root + SLASH + it->second.upload_directory + SLASH + file == req.path)
				return _DELETEmethod(req);
		}
		if (root + SLASH + it->second.root + SLASH + file == req.path ||
			root + SLASH + it->second.root + SLASH + it->second.upload_directory + SLASH + file == req.path)
			return _DELETEmethod(req);
	}
	return SetErrorResponse(404);
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
	_response.cgi_rep.clear();
}

void Server::SetResponse(int n)
{
	// std::cout << "req.path = " << _requests[n].path << std::endl;
	// std::cout << _requests[n].method << std::endl;
	location loc;
	int allowed = allowedMethod(_requests[n], loc);
	// std::cout << "HERE >" << allowed << std::endl;
	if (allowed != 200 && allowed != 302)
	{
		SetErrorResponse(allowed);
		return;
	}
	if (_requests[n].method == GET)
		_responseGET(_requests[n], loc);
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
	if (pos == std::string::npos)
		return 1;
	offset = pos + 10;
	pos = img.find("\"", offset);
	filename = img.substr(offset, pos - offset);
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
		pos = body.rfind("\n", pos);
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
	_requests[n].path.replace(0, 1, _conf.root + SLASH);
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
