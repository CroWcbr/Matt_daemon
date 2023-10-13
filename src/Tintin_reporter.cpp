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
	return true;
}

std::string Tintin_reporter::getCurrentDateTime()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "[%d/%m/%Y-%H:%M:%S]", &tstruct);
	return buf;
}

std::string Tintin_reporter::getCurrentDateTimeArchive()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y%m%d-%H:%M:%S-", &tstruct);
	return buf;
}

void Tintin_reporter::log(LogType type, const std::string& message)
{
	if (createLogDirectory())
	{
		std::ofstream logStream;
		std::string logFilePath = std::string(LOG_DIRECTORY) + "/" + LOG_FILENAME;

		std::lock_guard<std::mutex> lock(_mutex);
		logStream.open(logFilePath, std::ios::app);

		if (logStream.is_open())
		{
			std::string logPrefix;
			if (type == INFO)
				logPrefix = "[INFO]";
			else if (type == WARNING)
				logPrefix = "[WARNING]";
			else if (type == ERROR)
				logPrefix = "[ERROR]";
			else if (type == DEBUG)
				logPrefix = "[DEBUG]";

			logStream << getCurrentDateTime() << " " << logPrefix << " - Matt_daemon: " << message << std::endl;
		}
		else
			std::cerr << "Failed to open log file." << std::endl;
	}
}

void Tintin_reporter::makeArchive()
{
	int status;
	std::string oldFileName = std::string(LOG_DIRECTORY) + "/" + LOG_FILENAME;
	std::string newFileName = std::string(LOG_DIRECTORY) + "/" + getCurrentDateTimeArchive() + LOG_FILENAME ;
	{
		std::lock_guard<std::mutex> lock(_mutex);
		status = std::rename(oldFileName.c_str(), newFileName.c_str());
	}
	if (!status)
		log(INFO, "File renamed successfully : " + newFileName);
	else
		log(ERROR, "Failed to Archive the file.");
}
