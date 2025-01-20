#include <sys/epoll.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include "../includes/webserv.hpp"

 #define MAX_EVENTS 28


int main(){

struct epoll_event ev, events[MAX_EVENTS];
struct sockaddr_in			addr;
int listen_sock, conn_sock, nfds, epollfd;

	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == -1) {
		perror("socket");
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;//inet_addr("0.0.0.0");
	addr.sin_port = htons(8080);
	int b = true;

	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEPORT, &b, sizeof(int));
	if (bind(listen_sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
	{
		perror("bind");
		close(listen_sock);
		return (1);
	}
	if (listen(listen_sock, 10) == -1)
	{
		perror("listen");
		close(listen_sock);
		return (1);
	}

epollfd = epoll_create1(28);  // Create an epoll instance with a unique flag (28).
if (epollfd == -1) {         // Check if epoll creation failed.
    perror("epoll_create1"); // Print the error if epoll creation failed.
}

ev.events = EPOLLIN;        // Set the event type to EPOLLIN (ready for reading).
ev.data.fd = listen_sock;   // Associate the listening socket with the event.
if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
    // Add the listening socket to the epoll instance and check for errors.
    perror("epoll_ctl: listen_sock"); // Print error message if the operation fails.
}

for (;;) { // Infinite loop to handle incoming events.
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    // Wait for events on the epoll instance. Returns the number of ready file descriptors.
    if (nfds == -1) {        // Check if epoll_wait encountered an error.
        perror("epoll_wait"); // Print the error message.
    }

    for (int n = 0; n < nfds; ++n) { // Iterate over the ready file descriptors.
        if (events[n].data.fd == listen_sock) {
            // If the ready file descriptor is the listening socket,
            // accept a new incoming connection.
            conn_sock = accept(listen_sock, (struct sockaddr *) &addr, (socklen_t *)&sizeof(addr));
            if (conn_sock == -1) { // Check if accept failed.
                perror("accept");  // Print the error message.
            }
            setnonblocking(conn_sock); // Set the new connection socket to non-blocking mode.
            ev.events = EPOLLIN | EPOLLET; // Set event type to EPOLLIN with edge-triggered behavior.
            ev.data.fd = conn_sock;      // Associate the new connection socket with the event.
            if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
                // Add the new connection socket to the epoll instance and check for errors.
                perror("epoll_ctl: conn_sock"); // Print error message if the operation fails.
            }
        } else {
            // If the ready file descriptor is not the listening socket,
            // handle the event for the corresponding connection.
            do_use_fd(events[n].data.fd);
        }
    }
}
}
