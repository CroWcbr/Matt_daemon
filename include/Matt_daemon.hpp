#pragma once

#define LOCK_FILE "/var/lock/matt_daemon.lock"

class Matt_daemon
{
private:

public:
	Matt_daemon() = delete;
	Matt_daemon(Matt_daemon const &copy) = delete;
	Matt_daemon &operator=(Matt_daemon const &copy) = delete;

	Matt_daemon(int argc, char **argv);
	~Matt_daemon();

	static bool Check_lock(); 
};
