#include "../include/Tintin_reporter.hpp"
#include "../include/Matt_daemon.hpp"
#include "../include/Server.hpp"

Matt_daemon::Matt_daemon(int argc, char **argv)
{
	Tintin_reporter::log(Tintin_reporter::DEBUG, "test");
	Tintin_reporter::log(Tintin_reporter::INFO, "test");

	Tintin_reporter::makeArchive();
	Tintin_reporter::log(Tintin_reporter::ERROR, "test");
	Tintin_reporter::log(Tintin_reporter::WARNING, "test");
	// if (!Check_lock())
	// {

	// 	exit(1);
	// }

}

Matt_daemon::~Matt_daemon()
{

}

bool	Matt_daemon::Check_lock()
{
	// int lockFileDescriptor = open(lockFile, O_RDWR | O_CREAT, 0644);
	return 1;
}

// Matt_daemon::Start(int argc, char **argv)
// {
// 	Server serve(argc, argv);

// 	serve.Loop();
// }
