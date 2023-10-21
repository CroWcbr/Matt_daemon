#include "../include/Matt_daemon.hpp"

#include <iostream>
#include <unistd.h>

#include "../include/Tintin_reporter.hpp"

int main(int argc, char **argv)
{
	if (getuid() != 0)
	{
		std::cerr << "Matt_daemon : You must be root to run this program" << std::endl;
		return 1;
	}

	Matt_daemon&  deamon = Matt_daemon::getInstance();

	deamon.StartServer(argc, argv);
	deamon.LoopPoll();

	return 0;
}
