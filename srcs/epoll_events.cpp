#include "../includes/Init.hpp"
#include "../includes/Server.hpp"
#include "../includes/webserv.hpp"
#include <algorithm>
#include <cstdlib>
#include <ostream>
#include <vector>

void redirect_serv(std::vector<Server> &serv, int &server_index, std::string &host)
{
	int i = 0;
	for (std::vector<Server>::iterator it = serv.begin(); it != serv.end(); it++)
	{
		if (it->server_name == host)
		{
			server_index = i;
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
		if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events.data.fd, &ev) == -1)
		{
			perror("Error deleting the current connection");
			return 1;
		}
		grb.erase(std::remove(grb.begin(), grb.end(), events.data.fd), grb.end());
		for (std::vector<Server>::iterator it = serv.begin(); it != serv.end(); it++)
			it->client_fd.erase(std::remove(it->client_fd.begin(), it->client_fd.end(), events.data.fd), it->client_fd.end());
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
		if (count == 0)
		{
			close(events.data.fd);
			for (std::vector<Server>::iterator it = serv.begin(); it != serv.end(); it++)
				it->client_fd.erase(std::remove(it->client_fd.begin(), it->client_fd.end(), events.data.fd), it->client_fd.end());
		}
		if (check_host(buff, host))
			return 1;
		redirect_serv(serv, serv_index, host);
		serv[serv_index].fillRequest(new_connexion, buff);
		serv[serv_index].SetResponse(new_connexion);
		// serv[serv_index].print_request(new_connexion);
		rep.push_back(serv[serv_index]._response);
		serv[serv_index].clear_response();
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
		// std::cout << rep[0].repInString() << RESET << std::endl;
		if (rep[0].cgi_rep.empty())
		{
			std::cout << rep[0].cgi_rep << std::endl << rep[0].cgi_rep.size() << std::endl;
			if (send(events.data.fd, rep[0].repInString().c_str(), rep[0].repInString().size(), MSG_NOSIGNAL) == -1)
				std::cerr << "Send error: " << std::endl;
		}
		else
		{
			std::cout << rep[0].cgi_rep << std::endl << rep[0].cgi_rep.size() << std::endl;
			// std::cout << "HERE" << RESET << std::endl;
			if (send(events.data.fd, rep[0].cgi_rep.c_str(), rep[0].cgi_rep.size(), MSG_NOSIGNAL) == -1)
				std::cerr << "Send error: " << std::endl;
		}
		rep.erase(rep.begin());
		struct epoll_event ev;
		ev.data.fd = events.data.fd;
		ev.events = EPOLLIN | EPOLLRDHUP;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events.data.fd, &ev) == -1)
		{
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
	grb.push_back(epoll_fd);
	evi.events = EPOLLIN;
	evi.data.fd = servs[0].server_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, servs[0].server_fd, &evi) == -1)
		perror("epoll_ctl : socket_fd");
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
				if (!std::count(servs[server_index].client_fd.begin(), servs[server_index].client_fd.end(), new_connexion))
				{
					evi.data.fd = new_connexion;
					evi.events = EPOLLIN | EPOLLERR | EPOLLRDHUP;
					if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, evi.data.fd, &evi) == -1)
					{
						std::cerr << "Failed to add new new_connexion to epoll: " << std::endl;
						close(new_connexion);
						continue;
					}
					servs[server_index].client_fd.push_back(evi.data.fd);
					grb.push_back(evi.data.fd);
				}
			}
			else
			{
				for (int i = 0; i < (int)servs.size(); i++)
					if (std::count(servs[i].client_fd.begin(), servs[i].client_fd.end(), events[i].data.fd))
						server_index = i;
				if (server_index == -1)
					continue;
				if (handle_epollrdhup(servs, events[i], epoll_fd))
					continue;
				if (handle_epollin(servs, events[i], epoll_fd, new_connexion, server_index, rep))
					continue;
				if (handle_epollout(servs[server_index], events[i], epoll_fd, rep))
					continue;
			}
		}
	}
	close(epoll_fd);
}
