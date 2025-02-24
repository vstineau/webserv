#include "../includes/Server.hpp"
#include "../includes/webserv.hpp"
#include <algorithm>
#include <cstdlib>
#include <sys/socket.h>

int handle_epollrdhup(Server &serv, struct epoll_event *events, int &n, int &epoll_fd) {
	struct epoll_event ev;
	if (events[n].events & EPOLLRDHUP) // if (a client leaves)
	{
		std::cout << "Client " << events[n].data.fd << " disconnected" << RESET << std::endl;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[n].data.fd, &ev) == -1) {
			perror("Error deleting the current connection");
			return 1;
		}
		// close(events[n].data.fd);
		serv.client_fd.erase(
			std::remove(serv.client_fd.begin(), serv.client_fd.end(), events[n].data.fd),
			serv.client_fd.end());
		std::cout << RED << "fd poulet [" << events[n].data.fd << "] closed" << RESET << std::endl;
		close(events[n].data.fd);
		return 1;
	}
	return 0;
}

int handle_epollin(Server &serv, struct epoll_event *events, int &n, int &epoll_fd) {
	if (events[n].events & EPOLLIN) {
		// print on terminal what the server receives
		int count = 0;
		char buffer[1025];
		std::string buff;
		do {
			count = recv(events[n].data.fd, buffer, 1024, 0);
			// std::string test(buffer);
			buff.append(buffer, count);
			// buff += test;
		} while (count == 1024);
		if (count == 0) {
			std::cout << "client " << events[n].data.fd << " disconnected" << std::endl;
			close(events[n].data.fd);
			serv.client_fd.erase(
				std::remove(serv.client_fd.begin(), serv.client_fd.end(), events[n].data.fd),
				serv.client_fd.end());
			std::cout << RED << "fd [" << events[n].data.fd << "] closed" << RESET << std::endl;
		}
		std::cout << HI_CYAN << "-----------REQUEST----------" << RESET << std::endl;
		serv.fillRequest(n, buff);
		// serv.identifyRequest(n);
		serv.print_request(n);
		serv.SetResponse(n);
		std::cout << HI_CYAN << "-----------REQUEST----------" << RESET << std::endl;
		// std::cout << buff << std::endl;
		struct epoll_event ev;
		ev.data.fd = events[n].data.fd;
		ev.events = EPOLLOUT;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, ev.data.fd, &ev) == -1) {
			perror("epoll_ctl : client_fd 2");
			return 1;
		}
		return 1;
	}
	return 0;
}

int handle_epollout(Server &serv, struct epoll_event *events, int &n, int &epoll_fd) {
	if (events[n].events & EPOLLOUT) {
		// if (send_response(fillDirectoryListing(directory_listing("www")), events[n].data.fd) ==
		// -1) 	std::cerr << "Send error: " << std::endl;
		std::cout << serv.getResponse() << RESET << std::endl;
		if (send(events[n].data.fd, serv.getResponse().c_str(), serv.getResponse().size(),
				 MSG_NOSIGNAL) == -1)
			std::cerr << "Send error: " << std::endl;
		struct epoll_event ev;
		ev.data.fd = events[n].data.fd;
		ev.events = EPOLLIN | EPOLLRDHUP;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[n].data.fd, &ev) == -1) {
			std::cout << ev.data.fd << "\n";
			perror("epoll_ctl : client_fd 3");
			return 1;
		}
		return 1;
	}
	return 0;
}

void epoll_loop(std::map<int, Server> map_serv) {
	int epoll_fd = epoll_create(1);
	struct epoll_event evi;
	struct epoll_event events[MAX_EVENTS];
	int new_connexion = -1;
	std::map<int , Server>::iterator it = map_serv.begin();
	if (epoll_fd == -1) {
		perror("epoll_create");
		// close(map_serv[0].server_fd);
		exit(1);
	}
	evi.events = EPOLLIN;
	evi.data.fd = map_serv[0].server_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, it->first, &evi) == -1) {
		perror("epoll_ctl : socket_fd");
		// close(map_serv[0].server_fd);
		exit(1);
	}
	while (!g_end) {
		int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if (event_count == -1) {
			perror("event_count");
			// close(map_serv[0].server_fd);
			continue;
		}
		for(std::map<int , Server>::iterator it = map_serv.begin(); it != map_serv.end(); it++)
			std::cout << it->first << RESET << std::endl;
		for (int n = 0; n < event_count; n++) {
			// std::cout << events[n].data.fd << RESET << std::endl;
			// exit(1)
			if(map_serv.count(events[n].data.fd)) { // if(std::count(servers[fd]))
				sockaddr_in client_addr;
				int addrlen = sizeof(client_addr);
				new_connexion =
					accept(map_serv[events[n].data.fd].server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen);
				if (new_connexion == -1) {
					perror("accept");
					close(map_serv[events[n].data.fd].server_fd);
					continue;
				}
				std::cout << YELLOW << "Accepted new connection with fd: " << new_connexion
						  << std::endl;
				if (!std::count(map_serv[events[n].data.fd].client_fd.begin(), map_serv[events[n].data.fd].client_fd.end(), new_connexion)) {
					std::cout << BLUE << "NEW CLIENT" << RESET << std::endl;
					evi.data.fd = new_connexion;
					evi.events = EPOLLIN | EPOLLERR | EPOLLRDHUP;
					if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, evi.data.fd, &evi) == -1) {
						std::cerr << "Failed to add new new_connexion to epoll: " << std::endl;
						close(new_connexion);
						std::cout << RED << "fd [" << new_connexion << "] closed" << RESET
								  << std::endl;
						continue;
					}
					map_serv[events[n].data.fd].client_fd.push_back(evi.data.fd);
					// map_serv[events[n].data.fd] = 
					std::cout << GREEN << "CLIEND ADDED IN VECTOR" << RESET << std::endl;
				}
			} else {
				// struct epoll_event ev;
				if (handle_epollrdhup(map_serv[events[n].data.fd], events, n, epoll_fd))
					continue;
				if (handle_epollin(map_serv[events[n].data.fd], events, n, epoll_fd))
					continue;
				if (handle_epollout(map_serv[events[n].data.fd], events, n, epoll_fd))
					continue;
			}
		}
	}
	close(epoll_fd);
}
