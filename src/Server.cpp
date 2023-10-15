#include "../include/Server.hpp"

#include <cstring>
#include <unistd.h>

Server::Server()
{
	connectedClients = 0;

}

Server::~Server() 
{
}

void Server::Start(int argc, char **argv)
{
	if (!_ServerStart())
		exit(EXIT_FAILURE);
}

bool Server::_ServerStart()
{
	if (MYDEBUG)
		std::cout << "_ServerStart" << std::endl;

	try
	{
		serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket < 0)
			throw std::runtime_error("Failed to create socket");

		memset(&serverAddr, 0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		serverAddr.sin_port = htons(PORT);

		if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
			throw std::runtime_error("Failed to bind socket");

		if (listen(serverSocket, MAX_CLIENTS) < 0)
			throw std::runtime_error("Failed to listen");

		struct pollfd tmp;
		tmp.fd = serverSocket;
		tmp.events = POLLIN;
		tmp.revents = 0;
		_fds.push_back(tmp);

		std::cout << "\033[92m";
		std::cout << " SERVER_START" << std::endl;
		std::cout << "\033[0m";
	}
	catch (std::exception& e)
	{
		std::cout << "\033[91m";
		std::cout << "ERROR SERVER START : " << e.what() << std::endl;
		std::cout << "\033[0m";
		return false;
	}
	return true;
}

void Server::Loop()
{
	if (MYDEBUG)
		std::cout << "Loop" << std::endl;

	while (true)
	{
		if (MYDEBUG)
			std::cout << "while Loop : " << connectedClients << "\t" << _fds.size() << std::endl;

		_PollWait();

		for (pollfdType::iterator it = _fds.begin(); it != _fds.end();)
		{
			std::cout << "\t" << it->events << "\t" << it->fd << "\t" << it->revents << std::endl;
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
	if (MYDEBUG)
		std::cout << "\033[92m" << "_PollWait" << "\033[0m" << std::endl;

	int pollResult = poll(_fds.data(), _fds.size(), -1);

	if (pollResult < 0)
	{
		std::cout << "POLL ERROR: poll = -1  " << std::endl;
		pollResult = 0;
	}
}

void Server::_PollInServ(pollfdType::iterator &it)
{
	if (MYDEBUG)
		std::cout << "_PollInServ " << std::endl;

	it->revents = 0;
	if (connectedClients < MAX_CLIENTS)
	{
		sockaddr_in	clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);

		int clientSocket = accept(it->fd, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
		if (clientSocket < 0)
			std::cout << "ACCEPT ERROR: could not accept new connection" << std::endl;
		else
		{
			pollfd clientPollFd;
			clientPollFd.fd = clientSocket;
			clientPollFd.events = POLLIN;
			_fds.push_back(clientPollFd);
			std::cout << "Client connected" << std::endl;
			++connectedClients;
		}
	}
	else
	{
		int rejectedSocket = accept(serverSocket, nullptr, nullptr);
		if (rejectedSocket < 0)
		{
			std::cout << "ACCEPT ERROR: could not accept new connection" << std::endl;
		}
		else
		{
			send(rejectedSocket, "No available slots. Try later.\n", 31, 0);
			close(rejectedSocket);
		}
	}
}

void Server::_PollInUser(pollfdType::iterator &it)
{
	if (MYDEBUG == 1)	
		std::cout << "_PollInUser" << it->fd << std::endl;

	char buffer[MAX_BUFFER_RECV];
	int bytesRead = recv(it->fd, buffer, MAX_BUFFER_RECV - 1, 0);
	std::cout << "!!!\t" << bytesRead << std::endl;
	if (bytesRead <= 0)
	{
		if (bytesRead == 0)
			std::cout << "Client disconnected" << std::endl;
		else
			std::cerr << "Failed to read from client" << std::endl;
		close(it->fd);
		it = _fds.erase(it);
		connectedClients--;
		return;
	}
	else
	{
		buffer[bytesRead] = '\0';
		std::string message(buffer);

		if (message == "quit") {
			std::cout << "Client requested to quit" << std::endl;
			close(it->fd);
			it = _fds.erase(it);
			connectedClients--;
			return;
		}
		else
		{
			message.push_back('\n');
			send(it->fd, message.c_str(), message.length(), 0);
			std::cout << message << std::endl;
		}
	}
	it++;
}

void Server::_PollElse(pollfdType::iterator &it)
{
	if (MYDEBUG == 1)		
		std::cout << "_PollElse" << it->fd << " : ";

	if (it->revents & POLLNVAL)
		std::cout << "SERVER_POLLNVAL" << std::endl;
	else if (it->revents & POLLHUP)
		std::cout << "SERVER_POLLHUP" << std::endl;
	else if (it->revents & POLLERR)
		std::cout << "SERVER_POLLERR" << std::endl;

	it = _fds.erase(it);
}
