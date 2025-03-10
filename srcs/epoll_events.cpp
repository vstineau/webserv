#include "../includes/Init.hpp"
#include "../includes/Server.hpp"
#include "../includes/webserv.hpp"
#include <algorithm>
#include <cstdlib>
#include <vector>

void redirect_serv(std::vector<Server> &serv, int &server_index, std::string &host)
{
	int i = 0;
	for (std::vector<Server>::iterator it = serv.begin(); it != serv.end(); it++)
	{
		std::cout << "host: " << host << "|server: " << i << " = " << it->server_name << std::endl;
		if (it->server_name == host)
		{
			server_index = i;
			std::cout << "1 server_index = " << server_index << std::endl;
			return;
		}
		i++;
	}
}

static int handle_epollrdhup(std::vector<Server> &serv, struct epoll_event &events, int &epoll_fd)
{
	struct epoll_event ev;
	if (events.events & EPOLLRDHUP) // if (a client leaves)
	{
		std::cout << "Client " << events.data.fd << " disconnected" << RESET << std::endl;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events.data.fd, &ev) == -1)
		{
			perror("Error deleting the current connection");
			return 1;
		}
		for(std::vector<Server>::iterator it = serv.begin(); it != serv.end(); it++)
			it->client_fd.erase(std::remove(it->client_fd.begin(), it->client_fd.end(), events.data.fd), it->client_fd.end());
		std::cout << RED << "fd poulet [" << events.data.fd << "] closed" << RESET << std::endl;
		close(events.data.fd);
		return 1;
	}
	return 0;
}

static int handle_epollin(std::vector<Server> &serv, struct epoll_event &events, int &epoll_fd, int &new_connexion,
						  int &serv_index, std::vector<response> &rep)
{
	if (events.events & EPOLLIN)
	{
		int count = 0;
		char buffer[1025];
		std::string host;
		std::string buff;
		do
		{
			count = recv(events.data.fd, buffer, 1024, 0);
			buff.append(buffer, count);
		} while (count == 1024);
		// if (count == 0)
		// {
		// 	std::cout << "client " << events[n].data.fd << " disconnected" << std::endl;
		// 	close(events[n].data.fd);
		// 	serv.client_fd.erase(std::remove(serv.client_fd.begin(), serv.client_fd.end(), events[n].data.fd),
		// 						 serv.client_fd.end());
		// 	std::cout << RED << "fd [" << events[n].data.fd << "] closed" << RESET << std::endl;
		// }
		if (check_host(buff, host))
			return 1;
		std::cout << "before server_index = " << serv_index << std::endl;
		redirect_serv(serv, serv_index, host);
		std::cout << "after server_index = " << serv_index << std::endl;
		serv[serv_index].fillRequest(new_connexion, buff);
		serv[serv_index].SetResponse(new_connexion);
		rep.push_back(serv[serv_index]._response);
		serv[serv_index].clear_response();
		std::cout << "---------------RESPONSE---------------" << RESET << std::endl;
		std::cout << BLUE << serv[serv_index].getResponse() << RESET << std::endl;
		std::cout << "---------------RESPONSE---------------" << RESET << std::endl;
		// serv[serv_index].print_request(new_connexion);
		// std::cout << HI_CYAN << "-----------REQUEST----------" << RESET << std::endl;
		struct epoll_event ev;
		ev.data.fd = events.data.fd;
		ev.events = EPOLLOUT;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, ev.data.fd, &ev) == -1)
		{
			perror("epoll_ctl : client_fd 2");
			return 1;
		}
		return 1;
	}
	return 0;
}

static int handle_epollout(Server &serv, struct epoll_event &events, int &epoll_fd, std::vector<response> &rep)
{
	if (events.events & EPOLLOUT)
	{
		serv.status_code = 200;
		if (send(events.data.fd, rep[0].repInString().c_str(), rep[0].repInString().size(), MSG_NOSIGNAL) == -1)
			std::cerr << "Send error: " << std::endl;
		rep.erase(rep.begin());
		struct epoll_event ev;
		ev.data.fd = events.data.fd;
		ev.events = EPOLLIN | EPOLLRDHUP;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events.data.fd, &ev) == -1)
		{
			std::cout << ev.data.fd << "\n";
			perror("epoll_ctl : client_fd 3");
			return 1;
		}
		return 1;
	}
	return 0;
}

static int get_serv_index(std::vector<Server> &servs, int event_fd)
{
	int i = 0;
	for (std::vector<Server>::iterator it = servs.begin(); it != servs.end(); it++)
	{
		if (it->server_fd == event_fd)
			return i;
		i++;
	}
	return (-1);
}

void Init::epoll_loop()
{
	int epoll_fd = epoll_create(1);
	struct epoll_event evi;
	struct epoll_event events[MAX_EVENTS];
	std::vector<response> rep;
	int new_connexion = -1;
	if (epoll_fd == -1)
		perror("epoll_create");
	evi.events = EPOLLIN;
	evi.data.fd = servs[0].server_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, servs[0].server_fd, &evi) == -1)
		perror("epoll_ctl : socket_fd");
	for (std::vector<Server>::iterator it = servs.begin(); it != servs.end(); it++)
	{
		// it.
		std::cout << "serv_fd = " << it->server_fd << RESET << std::endl;
	}
	while (!g_end)
	{
		int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if (event_count == -1)
			continue;
		for (int i = 0; i < event_count; i++)
		{
			int server_index = get_serv_index(servs, events[i].data.fd);
			if (server_index > -1)
			{
				sockaddr_in client_addr;
				int addrlen = sizeof(client_addr);
				new_connexion = accept(servs[server_index].server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen);
				if (new_connexion == -1)
				{
					perror("accept");
					continue;
				}
				std::cout << "new connexion = " << new_connexion << RESET << std::endl;
				std::cout << YELLOW << "Accepted new connection with fd: " << new_connexion << std::endl;
				if (!std::count(servs[server_index].client_fd.begin(), servs[server_index].client_fd.end(), new_connexion))
				{
					std::cout << BLUE << "NEW CLIENT" << RESET << std::endl;
					evi.data.fd = new_connexion;
					evi.events = EPOLLIN | EPOLLERR | EPOLLRDHUP;
					if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, evi.data.fd, &evi) == -1)
					{
						std::cerr << "Failed to add new new_connexion to epoll: " << std::endl;
						close(new_connexion);
						std::cout << RED << "fd [" << new_connexion << "] closed" << RESET << std::endl;
						continue;
					}
					servs[server_index].client_fd.push_back(evi.data.fd);
					std::cout << GREEN << "CLIEND ADDED IN VECTOR" << RESET << std::endl;
				}
			}
			else
			{
				for (int i = 0; i < (int)servs.size(); i++)
					if (std::count(servs[i].client_fd.begin(), servs[i].client_fd.end(), events[i].data.fd))
						server_index = i;
				if (server_index == -1)
					continue;
				std::cout << "rdhup server_index = " << server_index << std::endl;
				if (handle_epollrdhup(servs, events[i], epoll_fd))
					continue;
				std::cout << "epollin server_index = " << server_index << std::endl;
				if (handle_epollin(servs, events[i], epoll_fd, new_connexion, server_index, rep))
					continue;
				std::cout << "epollout server_index = " << server_index << std::endl;
				if (handle_epollout(servs[server_index], events[i], epoll_fd, rep))
					continue;
			}
		}
	}
	close(epoll_fd);
}
