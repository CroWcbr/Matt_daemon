#pragma once

#include "../include/Server.hpp"

// #define LOCK_FILE "./log/matt_daemon.lock"
#define LOCK_FILE "/var/lock/matt_daemon.lock"

class Matt_daemon
{
private:
	int		lockFileDescriptor;
	Server	server;

	Matt_daemon();

	bool	Lock_file(); 
	void	Unlock_file();
	void	Delete_lock_file();

public:
	Matt_daemon(Matt_daemon const &copy) = delete;
	Matt_daemon &operator=(Matt_daemon const &copy) = delete;
	~Matt_daemon();

	static Matt_daemon& getInstance()
	{
		static Matt_daemon instance;
		return instance;
	}

	void	StartServer();
	void	LoopPoll();
	void	Clear_all();
};
