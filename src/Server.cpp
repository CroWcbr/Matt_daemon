#include "../include/Server.hpp"
#include "../include/Tintin_reporter.hpp"

#include <cstring>
#include <unistd.h>

Server::Server()
{
	connectedClients = 0;
}

Server::~Server() 
{
	Tintin_reporter::log(Tintin_reporter::DEBUG, "Server::~Server() ");
	if (connectedClients)
		CloseAllConnection();
}

void Server::CloseAllConnection()
{
	if (_fds.empty())
		return;
	for (pollfdType::iterator it = _fds.begin() + 1; connectedClients && it != _fds.end();)
	{
		Tintin_reporter::log(Tintin_reporter::DEBUG, "Close connection fd : " + std::to_string(it->fd));
		close(it->fd);
		_authentication.erase(it->fd);
		it = _fds.erase(it);
		connectedClients--;
	}
	Tintin_reporter::log(Tintin_reporter::DEBUG, "Close server fd : " + std::to_string(_fds.begin()->fd));
	close(_fds.begin()->fd);
	_fds.erase(_fds.begin());
}

void Server::Start()
{
	Tintin_reporter::log(Tintin_reporter::INFO, "Creating server.");
	if (!_ServerStart())
		exit(EXIT_FAILURE);
	Tintin_reporter::log(Tintin_reporter::INFO, "Server created.");
}

bool Server::_ServerStart()
{
	try
	{
		serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket < 0)
			throw std::runtime_error("Failed to create socket");

		memset(&serverAddr, 0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		serverAddr.sin_port = htons(PORT);

		int reuse = 1;
		if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
			throw std::runtime_error("Failed to bind socket");

		if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
			throw std::runtime_error("Failed to bind socket");

		if (listen(serverSocket, MAX_CLIENTS) < 0)
			throw std::runtime_error("Failed to listen");

		struct pollfd tmp;
		tmp.fd = serverSocket;
		tmp.events = POLLIN;
		tmp.revents = 0;
		_fds.push_back(tmp);
	}
	catch (std::exception& e)
	{
		Tintin_reporter::log(Tintin_reporter::ERROR, "ERROR SERVER START : " + std::string(e.what()));
		return false;
	}
	return true;
}

void Server::Loop()
{
	while (true)
	{
		_PollWait();

		for (pollfdType::iterator it = _fds.begin(); it != _fds.end();)
		{
			if (it->revents == 0)
			{
				++it;
				continue;
			}

			if (it->revents & POLLIN && it->fd == serverSocket)
			{
				_PollInServ(it);
				break;
			}
			else if (it->revents & POLLIN)
				_PollInUser(it);
			// else if (it->revents & POLLOUT)
			// 	_PollOut(it);
			else
				_PollElse(it);
		}
	}
}

void Server::_PollWait()
{
	int pollResult = poll(_fds.data(), _fds.size(), -1);

	if (pollResult < 0)
	{
		Tintin_reporter::log(Tintin_reporter::ERROR, "POLL ERROR: poll = -1");
		pollResult = 0;
	}
}

void Server::_PollInServ(pollfdType::iterator &it)
{
	it->revents = 0;
	if (connectedClients < MAX_CLIENTS)
	{
		sockaddr_in	clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);

		int clientSocket = accept(it->fd, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
		if (clientSocket < 0)
			Tintin_reporter::log(Tintin_reporter::ERROR, "ACCEPT ERROR: could not accept new connection.");
		else
		{
			pollfd clientPollFd;
			clientPollFd.fd = clientSocket;
			clientPollFd.events = POLLIN;
			_fds.push_back(clientPollFd);
			_authentication[clientSocket] = false;
			send(clientSocket, "Input password!\n", 16, 0);
			Tintin_reporter::log(Tintin_reporter::INFO, "Client connected and wait authentication : " + std::to_string(clientSocket));
			++connectedClients;
		}
	}
	else
	{
		int rejectedSocket = accept(serverSocket, nullptr, nullptr);
		if (rejectedSocket < 0)
		{
			Tintin_reporter::log(Tintin_reporter::ERROR, "ACCEPT ERROR: could not accept new connection.");
		}
		else
		{
			Tintin_reporter::log(Tintin_reporter::INFO, "Client try connected, but no available slots.");
			send(rejectedSocket, "No available slots. Try later.\n", 31, 0);
			close(rejectedSocket);
		}
	}
}

void Server::_PollInUser(pollfdType::iterator &it)
{
	char buffer[MAX_BUFFER_RECV];
	int bytesRead = recv(it->fd, buffer, MAX_BUFFER_RECV - 1, 0);
	if (bytesRead <= 0)
	{
		if (bytesRead == 0)
			Tintin_reporter::log(Tintin_reporter::INFO, "Client disconnected : " + std::to_string(it->fd));
		else
			Tintin_reporter::log(Tintin_reporter::ERROR, "Failed to read from client : " + std::to_string(it->fd));
		
		close(it->fd);
		it = _fds.erase(it);
		_authentication.erase(it->fd);
		connectedClients--;
		return;
	}
	else
	{
		buffer[bytesRead] = '\0';
		std::string message(buffer);

		if (message.back() == '\n')
			message.pop_back();

		if (_authentication[it->fd] == false) // bonus
		{
			std::string send_mes;
			if (message == PASS)
			{
				send_mes = "Authentication OK!";
				Tintin_reporter::log(Tintin_reporter::INFO, send_mes + " : " + std::to_string(it->fd));
				send_mes += "\n";
				send(it->fd, send_mes.c_str(), send_mes.length(), 0);
				_authentication[it->fd] = true;
			}
			else
			{
				send_mes = "Wrong password. Disconect.\n";
				Tintin_reporter::log(Tintin_reporter::INFO, send_mes + " : " + std::to_string(it->fd));
				send_mes += "\n";
				send(it->fd, send_mes.c_str(), send_mes.length(), 0);
				close(it->fd);
				it = _fds.erase(it);
				_authentication.erase(it->fd);
				connectedClients--;
				return;
			}
		}
		else if (message == "quit")
		{
			Tintin_reporter::log(Tintin_reporter::INFO, "Request quit from : " + std::to_string(it->fd));
			CloseAllConnection();
			exit(0);
		}
		else if (message.substr(0, 3) == SHELL) // bonus
		{
			std::string sh_command = message.substr(3);
			std::string result;

			Tintin_reporter::log(Tintin_reporter::INFO, "Shell command : " + sh_command + " from : " + std::to_string(it->fd));
			FILE* cmdStream = popen(sh_command.c_str(), "r");
			if (cmdStream)
			{
				char cmdOutput[128];
				while (fgets(cmdOutput, sizeof(cmdOutput), cmdStream) != nullptr)
					result.append(cmdOutput);
				pclose(cmdStream);

			}
			else
				result = "Error in shell command";

			if (result.empty())
				result = "Command done!";
			Tintin_reporter::log(Tintin_reporter::INFO, "result Shell command : \n" + result);
			send(it->fd, result.c_str(), result.length(), 0);
		}
		else
		{
			Tintin_reporter::log(Tintin_reporter::LOG, "User " + std::to_string(it->fd) + " input : "+ message);

			///fot test
			// message += "\n";
			// send(it->fd, message.c_str(), message.length(), 0);
		}
	}
	it++;
}

void Server::_PollElse(pollfdType::iterator &it)
{
	std::string messenge = "_PollElse " + std::to_string(it->fd) + " : ";

	if (it->revents & POLLNVAL)
		messenge += "SERVER_POLLNVAL";
	else if (it->revents & POLLHUP)
		messenge += "SERVER_POLLHUP";
	else if (it->revents & POLLERR)
		messenge += "SERVER_POLLERR";

	Tintin_reporter::log(Tintin_reporter::ERROR, messenge);
	close(it->fd);
	it = _fds.erase(it);
	_authentication.erase(it->fd);
}
