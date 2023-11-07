#pragma once

# include <vector>
# include <poll.h>
# include <arpa/inet.h>
# include <map>
# include <string>

#define PORT				4242	// port
#define PASS				"12345"	// password
#define SHELL				"sh "	// shell
#define MAX_CLIENTS			3		// max client
#define MAX_BUFFER_RECV		65535	// max possible read
#define SERV_DEBUG			1		// for debugging server

class Server
{
private:
	int									serverSocket;
	struct sockaddr_in					serverAddr;

	typedef std::vector<struct pollfd>	pollfdType;
	pollfdType							_fds;
	std::map<int, bool>					_authentication;

	int									connectedClients;

private:
	bool	_ServerStart();
	void	_PollWait();
	void	_PollInServ(pollfdType::iterator &it);
	void	_PollInUser(pollfdType::iterator &it);
	void	_PollElse(pollfdType::iterator &it);
	void	_ShellCommand(std::string& sh_command, int fd);

public:
	Server();
	Server(Server const &copy) = delete;
	Server &operator=(Server const &copy) = delete;
	~Server();

	void	Start();
	void	Loop();
	void	CloseAllConnection();
};
