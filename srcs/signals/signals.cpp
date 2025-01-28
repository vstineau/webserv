#include "../../includes/webserv.hpp"
#include "../../includes/Server.hpp"
#include <signal.h>

//catch the CTRL + 'C' set g_end on true to exit properly the program
static void	handle_sigint(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	(void)signum;
	g_end = true;
}

//set up the signals to catch the ctrl c 
int	init_signals()
{
	struct sigaction sig;

	sigemptyset(&sig.sa_mask);
	sig.sa_sigaction = handle_sigint;
	sig.sa_flags = SA_SIGINFO;
	if (sigaction(SIGINT, &sig, NULL) == -1)
		return (1);
	return (0);
}
