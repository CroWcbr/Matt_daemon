#include "../include/Tintin_reporter.hpp"
#include "../include/Matt_daemon.hpp"
#include "../include/Server.hpp"

#include <unistd.h>
#include <signal.h>
#include <thread>
#include <chrono>

void signal_handler(int signum)
{
	Tintin_reporter::log(Tintin_reporter::DEBUG, "sigterm_handler");

	// Matt_daemon&  deamon = Matt_daemon::getInstance();

	// deamon.clear_all();
	// Tintin_reporter::log(Tintin_reporter::DEBUG, "sigterm_handler22");
	exit(0);
}

void archiveTask()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(60));
		Tintin_reporter::makeArchive();
	}
}

Matt_daemon::Matt_daemon()
{
	if (!Tintin_reporter::createLogDirectory())
		exit(1);
	Tintin_reporter::log(Tintin_reporter::INFO, "Started.");

	pid_t pid = fork();
	if (pid == -1)
	{
		Tintin_reporter::log(Tintin_reporter::ERROR, "fork() == -1");
		std::cerr << "fork() == -1" << std::endl;
		exit(1);
	}

	if (pid == 0)
	{
		if (!Lock_file())
			exit(1);

		pid_t s_sid = setsid();
		if (s_sid == -1)
		{
			Tintin_reporter::log(Tintin_reporter::ERROR, "setsid() == -1");
			exit(1);
		}

		signal(SIGINT, signal_handler);
		signal(SIGQUIT, signal_handler);
		signal(SIGTERM, signal_handler);

		std::thread archiveThread(archiveTask);
		archiveThread.detach();

		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}
	else
	{
		exit(0);
	}
}

Matt_daemon::~Matt_daemon()
{
	clear_all();
	Tintin_reporter::log(Tintin_reporter::DEBUG, "Matt_daemon::~Matt_daemon()");
}

void Matt_daemon::clear_all()
{
	Tintin_reporter::log(Tintin_reporter::DEBUG, "Matt_daemon::clear_all()");
	Unlock_file();
	Delete_lock_file();
}

void	Matt_daemon::Delete_lock_file()
{
	if (unlink(LOCK_FILE) != 0)
		Tintin_reporter::log(Tintin_reporter::ERROR, "Error deleting lock file.");
	else
		Tintin_reporter::log(Tintin_reporter::DEBUG, "FILE_DELETE - " + std::string(LOCK_FILE));
}

void	Matt_daemon::Unlock_file()
{
	Tintin_reporter::log(Tintin_reporter::DEBUG, "Unlock_file.");
	if (flock(lockFileDescriptor, LOCK_UN) == -1)
		Tintin_reporter::log(Tintin_reporter::ERROR, "Error unlocking file.");
	else
		Tintin_reporter::log(Tintin_reporter::DEBUG, "FILE_UNLOCKED - " + std::string(LOCK_FILE));
	close(lockFileDescriptor);
}

bool	Matt_daemon::Lock_file()
{
	lockFileDescriptor = open(LOCK_FILE, O_CREAT, 0644);
	if (lockFileDescriptor == -1)
	{
		std::string messenge("Failed to open lock file.");
		Tintin_reporter::log(Tintin_reporter::ERROR, messenge);
		std::cerr << messenge << std::endl;
		return false;
	}

	if (flock(lockFileDescriptor, LOCK_EX | LOCK_NB) == -1)
	{
		std::string messenge("Another instance of the program is already running.");
		Tintin_reporter::log(Tintin_reporter::ERROR, messenge);
		std::cerr << messenge << std::endl;
		close(lockFileDescriptor);
		return false;
	}

	Tintin_reporter::log(Tintin_reporter::DEBUG, "FILE_LOCKED - " + std::string(LOCK_FILE));
	return true;
}

// Matt_daemon::Start(int argc, char **argv)
// {
// 	Server serve(argc, argv);

// 	serve.Loop();
// }

void Matt_daemon::Loop()
{
	Tintin_reporter::log(Tintin_reporter::DEBUG, "Loop");
	while(true)
	{
			Tintin_reporter::log(Tintin_reporter::ERROR, "BEFORLoop");
			std::this_thread::sleep_for(std::chrono::seconds(10));
			Tintin_reporter::log(Tintin_reporter::ERROR, "AFTERLoop");
	}
	Tintin_reporter::log(Tintin_reporter::DEBUG, "LoopEND");
}
