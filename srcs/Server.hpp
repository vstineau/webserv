
#ifndef SERVER_HPP
# define SERVER_HPP

#include "../includes/webserv.hpp"

class Server {

public:
	Server();
	Server(config conf_file);
	Server(Server const & src);
	~Server();
	Server &operator=(Server const & src);

private:
	config								 _conf;
	std::map<int, request> _requests;
};

#endif
