#include "../includes/Init.hpp"
#include "../includes/Server.hpp"
#include "../includes/webserv.hpp"
#include <algorithm>
#include <cstdlib>
#include <vector>

static int handle_epollrdhup(Server &serv, struct epoll_event *events, int &n, int &epoll_fd)
{
	struct epoll_event ev;
	if (events[n].events & EPOLLRDHUP) // if (a client leaves)
	{
		std::cout << "Client " << events[n].data.fd << " disconnected" << RESET << std::endl;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[n].data.fd, &ev) == -1)
		{
			perror("Error deleting the current connection");
			return 1;
		}
		serv.client_fd.erase(std::remove(serv.client_fd.begin(), serv.client_fd.end(), events[n].data.fd), serv.client_fd.end());
		std::cout << RED << "fd poulet [" << events[n].data.fd << "] closed" << RESET << std::endl;
		close(events[n].data.fd);
		return 1;
	}
	return 0;
}

static int handle_epollin(Server &serv, struct epoll_event *events, int &n, int &epoll_fd, int &new_connexion)
{
	if (events[n].events & EPOLLIN)
	{
		// print on terminal what the server receives
		int count = 0;
		char buffer[1025];
		std::string buff;
		do
		{
			count = recv(events[n].data.fd, buffer, 1024, 0);
			// std::string test(buffer);
			buff.append(buffer, count);
			// buff += test;
		} while (count == 1024);
		if (count == 0)
		{
			std::cout << "client " << events[n].data.fd << " disconnected" << std::endl;
			close(events[n].data.fd);
			serv.client_fd.erase(std::remove(serv.client_fd.begin(), serv.client_fd.end(), events[n].data.fd),
								 serv.client_fd.end());
			std::cout << RED << "fd [" << events[n].data.fd << "] closed" << RESET << std::endl;
		}
		// std::cout << HI_CYAN << "-----------REQUEST----------" << RESET << std::endl;
		serv.fillRequest(new_connexion, buff);
		// serv.identifyRequest(n);
		serv.SetResponse(new_connexion);
		//serv.print_request(n);
		std::cout << HI_CYAN << "-----------REQUEST----------" << RESET << std::endl;
		// std::cout << buff << std::endl;
		struct epoll_event ev;
		ev.data.fd = events[n].data.fd;
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

static int handle_epollout(Server &serv, struct epoll_event *events, int &n, int &epoll_fd)
{
	if (events[n].events & EPOLLOUT)
	{
		// std::cout << "---------------RESPONSE---------------" << RESET << std::endl;
		//  std::cout << BLUE << serv.getResponse() << RESET << std::endl;
		// std::cout << "---------------RESPONSE---------------" << RESET << std::endl;
		serv.status_code = 200;
		if (send(events[n].data.fd, serv.getResponse().c_str(), serv.getResponse().size(), MSG_NOSIGNAL) == -1)
			std::cerr << "Send error: " << std::endl;
		serv.clear_response();
		struct epoll_event ev;
		ev.data.fd = events[n].data.fd;
		ev.events = EPOLLIN | EPOLLRDHUP;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[n].data.fd, &ev) == -1)
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
	int new_connexion = -1;
	if (epoll_fd == -1)
		perror("epoll_create");
	evi.events = EPOLLIN;
	evi.data.fd = servs[0].server_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, servs[0].server_fd, &evi) == -1)
		perror("epoll_ctl : socket_fd");
	for (std::vector<Server>::iterator it = servs.begin(); it != servs.end(); it++)
		std::cout << "serv_fd = " << it->server_fd << RESET << std::endl;
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
				if (handle_epollrdhup(servs[server_index], events, i, epoll_fd))
					continue;
				if (handle_epollin(servs[server_index], events, i, epoll_fd, new_connexion))
					continue;
				if (handle_epollout(servs[server_index], events, i, epoll_fd))
					continue;
			}
		}
	}
	close(epoll_fd);
}
