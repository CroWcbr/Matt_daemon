#pragma once

#include <iostream>
#include <fstream>
#include <sys/file.h>
#include <sys/stat.h>
#include <string>
#include <ctime>
#include <sstream>
#include <mutex>

#define LOG_DIRECTORY	"./log"
#define LOG_FILENAME	"matt_daemon.log"
#define DEBUG_LOG		1

class Tintin_reporter
{
private:
	static std::mutex	_mutex;


	static std::string	_getCurrentDateTime();
	static std::string	_getCurrentDateTimeArchive();

public:
	enum LogType
	{
		INFO,
		LOG,
		ERROR,
		DEBUG
	};

	Tintin_reporter() = delete;
	~Tintin_reporter() = delete;
	Tintin_reporter(Tintin_reporter const &copy) = delete;
	Tintin_reporter &operator=(Tintin_reporter const &copy) = delete;

	static bool			createLogDirectory();
	static void			log(LogType type, const std::string& message);
	static void			makeArchive();
};
