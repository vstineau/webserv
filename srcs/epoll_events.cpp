#include <sys/epoll.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include "../includes/Server.hpp"
#include "../includes/webserv.hpp"

 #define MAX_EVENTS 28

void epollinit(Server &serv);
void epollin(Server &serv);
void epollrdup(Server &serv);
void epoll_loop(Server &serv, struct epoll_event &ev, struct epoll_event events[MAX_EVENTS]);

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
		close(serv.getServer_fd());
		exit(1);
	}
	ev.events = EPOLLIN;
	ev.data.fd = serv.getServer_fd();
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv.getServer_fd(), &ev) == -1)
	{
		perror("epoll_ctl : socket_fd");
		close(serv.getServer_fd());
		exit(1);
	}
	epoll_loop(serv, ev, events);
	close(serv.getServer_fd());
}

void epoll_loop(Server &serv, struct epoll_event &ev, struct epoll_event events[MAX_EVENTS])
{
	while (42)
	{
		int	event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if (event_count == -1)
		{
			perror("event_count");
			close(socket_fd);
			continue ;
		}
		for (int n = 0; n < event_count; n++)
		{
			if (events[n].data.fd == socket_fd) //if (new connection)
			{
				std::cout << "waiting for epoll\n";
				socklen_t client_addr_len = sizeof(client_addr);
				//accept a connection (stock the socket into an fd)
				client_fd = accept(socket_fd, (sockaddr*)&client_addr, &client_addr_len);
				if (client_fd == -1)
				{
					perror("accept");
					close(socket_fd);
					continue ;
				}
				// fill struct for epoll
				struct epoll_event ev;
				ev.events = EPOLLIN | EPOLLRDHUP;
				ev.data.fd = client_fd;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1)
				{
					perror("epoll_ctl : client_fd 1");
					exit(1);
				}
			}
			else
			{
				if (events[n].events & EPOLLRDHUP) // if (a client leaves)
				{
					if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[n].data.fd, &ev) == -1)
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
					do
					{
						i = read(events[n].data.fd, buffer, 1024);
						buffer[i] = '\0';
						printf("%s", buffer);
					} while (i == 1024);
					struct epoll_event ev;
					ev.data.fd = events[n].data.fd;
					ev.events = EPOLLOUT;
					//simulate response from server
					write(events[n].data.fd, "HTTP/1.1 200 OK\r\n", 17);
					// Content-Type: text/html
					write(events[n].data.fd, "Content-Type: text/html\r\n", 25);
					write(events[n].data.fd, "\r\n", 2);
					write(events[n].data.fd, "<head>\n\t<title>Hello world</title>\n</head>", 43);
					write(events[n].data.fd, "<h1>Hello world</h1>\n", 22);
					write(events[n].data.fd, "<p style='color: red;'>This is a paragraph</p>\n", 48);
					write(events[n].data.fd, "<a href=\"https://www.youtube.com/watch?v=MtN1YnoL46Q&pp=ygUNdGhlIGR1Y2sgc29uZw%3D%3D\">DUCK</a>\n", 96);
					if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, ev.data.fd, &ev) == -1)
					{
						perror("epoll_ctl : client_fd 2");
						continue ;
					}
				}
				if (events[n].events & EPOLLOUT)
				{
					//close (events[n].data.fd);
					struct epoll_event ev;
					ev.data.fd = events[n].data.fd;
					ev.events = EPOLLIN | EPOLLRDHUP;
					if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[n].data.fd, &ev) == -1)
					{
						std::cout << ev.data.fd << "\n";
						perror("epoll_ctl : client_fd 3");
						continue ;
					}
				}
			}
			}
		}
	}
}


