#include "../include/Matt_daemon.hpp"

#include <iostream>
#include <unistd.h>

#include "../include/Tintin_reporter.hpp"

int main()
{
	if (getuid() != 0)
	{
		std::cerr << "Matt_daemon : You must be root to run this program" << std::endl;
		return 1;
	}

	Matt_daemon&  deamon = Matt_daemon::getInstance();

	deamon.StartServer();
	deamon.LoopPoll();

	return 0;
}
