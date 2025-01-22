#include "../includes/Server.hpp"
#include "../includes/webserv.hpp"

void epollinit(Server &serv)
{
	int	epoll_fd;
	// fill struct for epoll
	struct epoll_event ev;
	struct epoll_event events[MAX_EVENTS];
	//setup epoll to 
	epoll_fd = epoll_create(1);
	if (epoll_fd == -1)
	{
		perror("epoll_create");
		close(serv.server_fd);
		exit(1);
	}
	ev.events = EPOLLIN;
	ev.data.fd = serv.server_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv.server_fd, &ev) == -1)
	{
		perror("epoll_ctl : socket_fd");
		close(serv.server_fd);
		exit(1);
	}
	epoll_loop(serv, ev, events, epoll_fd);
	close(serv.server_fd);
}

void epoll_loop(Server &serv, struct epoll_event &evi, struct epoll_event events[MAX_EVENTS], int epoll_fd)
{
	while (42)
	{
		int	event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if (event_count == -1)
		{
			perror("event_count");
			close(serv.server_fd);
			continue ;
		}
		for (int n = 0; n < event_count; n++)
		{
			if (events[n].data.fd == serv.server_fd) //if (new connection)
			{
				//accept a connection (stock the socket into an fd)
				sockaddr_in client_addr;
				int addrlen = sizeof(client_addr);
				serv.client_fd = accept(serv.server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen);
				if (serv.client_fd == -1)
				{
					perror("accept");
					close(serv.server_fd);
					continue ;
				}
				// fill struct for epoll
				struct epoll_event ev;
				ev.events = EPOLLIN | EPOLLRDHUP;
				ev.data.fd = serv.client_fd;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv.client_fd, &ev) == -1)
				{
					perror("epoll_ctl : client_fd 1");
					exit(1);
				}
			}
			else
			{
				if (events[n].events & EPOLLRDHUP) // if (a client leaves)
				{
					if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[n].data.fd, &evi) == -1)
					{
						perror("Error deleting the current connection");
						continue ;
					}
					close(events[n].data.fd);
					continue ;
				}
				if (events[n].events & EPOLLIN)
				{
					//print on terminal what the server receives
					int	i = 0;
					char	buffer[1025];
					std::string buff;
					do
					{
						i = read(events[n].data.fd, buffer, 1024);
						buffer[i] = '\0';
						buff += buffer;
					} while (i == 1024);
					serv.fillRequest(n, buff);
					serv.print_request(n);
					struct epoll_event ev;
					ev.data.fd = events[n].data.fd;
					ev.events = EPOLLOUT;
					if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, ev.data.fd, &ev) == -1)
					{
						perror("epoll_ctl : client_fd 2");
						continue ;
					}
					continue ;
				}
				if (events[n].events & EPOLLOUT)
				{
					std::string http_response =
					"HTTP/1.1 200 OK\r\n"  // Ligne de statut HTTP
					"Content-Type: text/html\r\n"  // Type de contenu (HTML)
					"Content-Length: 20\r\n"  // taille du contenu de la reponse
					"\r\n"  // Fin des en-têtes
					"AAAAAAAAAAAA Webserv";  // Corps de la réponse
					send(serv.client_fd, http_response.c_str(), http_response.size(), 0);
					struct epoll_event ev;
					ev.data.fd = events[n].data.fd;
					ev.events = EPOLLIN | EPOLLRDHUP;
					if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[n].data.fd, &ev) == -1)
					{
						std::cout << ev.data.fd << "\n";
						perror("epoll_ctl : client_fd 3");
						continue ;
					}
					continue ;
				}
			}
			}
		}
}


