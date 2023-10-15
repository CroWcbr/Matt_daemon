#pragma once

# include <iostream>
# include <vector>
# include <poll.h>
# include <arpa/inet.h>

#define PORT				4242	// port
#define MAX_CLIENTS			3		// max client
#define MAX_BUFFER_RECV		65535	// max possible read
#define MYDEBUG				1		// for debugging server

class Server
{
private:
	int									serverSocket;
	struct sockaddr_in					serverAddr;

	typedef std::vector<struct pollfd>	pollfdType;
	pollfdType							_fds;

	int									connectedClients;

private:
	bool	_ServerStart();
	void	_PollWait();
	void	_PollInServ(pollfdType::iterator &it);
	void	_PollInUser(pollfdType::iterator &it);
	void	_PollElse(pollfdType::iterator &it);

public:
	Server();
	Server(Server const &copy) = delete;
	Server &operator=(Server const &copy) = delete;
	~Server();

	void	Start(int argc, char **argv);
	void	Loop();
};
