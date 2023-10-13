#pragma once

#include <iostream>
#include <fstream>
#include <sys/file.h>
#include <sys/stat.h>
#include <string>
#include <ctime>
#include <sstream>
#include <mutex>

#define LOG_DIRECTORY "./log"
#define LOG_FILENAME "matt_daemon.log"

class Tintin_reporter
{
private:
	static std::mutex	_mutex;

	static bool			createLogDirectory();
	static std::string	getCurrentDateTime();
	static std::string	getCurrentDateTimeArchive();

public:
	enum LogType
	{
		INFO,
		WARNING,
		ERROR,
		DEBUG
	};

	static void			log(LogType type, const std::string& message);
	static void			makeArchive();
};
