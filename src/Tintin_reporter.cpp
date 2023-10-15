#include "../include/Tintin_reporter.hpp"

std::mutex Tintin_reporter::_mutex;

bool Tintin_reporter::createLogDirectory()
{
	struct stat st;
	if (stat(LOG_DIRECTORY, &st) != 0)
	{
		if (mkdir(LOG_DIRECTORY, 0755) != 0)
		{
			std::cerr << "Failed to create log directory." << std::endl;
			return false;
		}
	}
	else if (!S_ISDIR(st.st_mode))
	{
		std::cerr << "Log directory is not a directory." << std::endl;
		return false;
	}
	log(DEBUG, std::string("CHECK is OK LOG_DIRECTORY - ") + LOG_DIRECTORY);
	return true;
}

void Tintin_reporter::log(LogType type, const std::string& message)
{
	if (type == DEBUG && !DEBUG_LOG)
		return;

	std::ofstream logStream;
	std::string logFilePath = std::string(LOG_DIRECTORY) + "/" + LOG_FILENAME;

	std::lock_guard<std::mutex> lock(_mutex);
	logStream.open(logFilePath, std::ios::app);

	if (logStream.is_open())
	{
		std::string info;
		if (type == INFO)
			info = "[INFO]";
		else if (type == LOG)
			info = "[LOG]";
		else if (type == ERROR)
			info = "[ERROR]";
		else if (type == DEBUG)
			info = "[DEBUG]";

		logStream << _getCurrentDateTime() << " " << info << " - Matt_daemon: " << message << std::endl;
	}
	else
		std::cerr << "Failed to open log file." << std::endl;
}

void Tintin_reporter::makeArchive()
{
	int status;
	std::string oldFileName = std::string(LOG_DIRECTORY) + "/" + LOG_FILENAME;
	std::string newFileName = std::string(LOG_DIRECTORY) + "/" + _getCurrentDateTimeArchive() + LOG_FILENAME ;
	{
		std::lock_guard<std::mutex> lock(_mutex);
		status = std::rename(oldFileName.c_str(), newFileName.c_str());
	}
	if (!status)
		log(INFO, "File removed to archive - " + newFileName);
	else if (errno != ENOENT)
		log(ERROR, "Failed to Archive the file - " + oldFileName);
}

std::string Tintin_reporter::_getCurrentDateTime()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "[%d/%m/%Y-%H:%M:%S]", &tstruct);
	return buf;
}

std::string Tintin_reporter::_getCurrentDateTimeArchive()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y%m%d-%H:%M:%S-", &tstruct);
	return buf;
}
