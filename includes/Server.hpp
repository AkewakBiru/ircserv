/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 21:54:23 by abiru             #+#    #+#             */
/*   Updated: 2023/10/27 14:38:33 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <sys/types.h>
#include <netdb.h>
#include <string>
#include <sstream>
#include <ctime>
#include <climits>
#include <limits>
#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "Parser.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "IrcUtils.hpp"
#include "Commands.hpp"
#include "ResponseCodes.hpp"

#define MAX_CLIENTS 1000

// colors
#define RESET "\033[0m"
#define PURPLE "\033[35m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

class Client;

class Channel;

enum
{
	RUNNING,
	STOPPED
};

class Server
{
public:
	Server(std::string pass, int const port);
	~Server();
	void setPass(std::string &newPass);
	int getPort() const;
	std::string const &getPass() const;
	void checkParams() const;

	void setServFd(int const fd);
	int getServFd() const;

	void setServCreationTime();
	std::string &getServCreationTime();

	bool getServAddrInfo();
	bool createSocket();
	bool listenForConn() const;
	bool handleRequest();

	bool start();

	bool deleteConnection(Client *client);

	std::vector<Client *> const &getClients() const;
	std::vector<Channel *> const &getChannels() const;

	void addChannel(Channel *channel);
	void addClient(Client *client);
	void removeClient(Client *client);
	bool isRegistered(int fd);
	bool registerUser(Client *client, std::vector<std::string> const &res);
	std::vector<Client *>::iterator findFd(std::vector<Client *> &client, int fd);
	ssize_t getNicksFd(std::string nick);

	void sendWelcomingMsg(Client *client);

	void setStatus(bool status);
	int getStatus() const;

	void acceptNewConnection(void);
	void addNewClient(int fd, struct sockaddr_storage *client_addr);

	void cleanup();
	void removeNonRespClients();
	void removeEmptyChannels();
	void removeClients();

	void processBuffer(Client *client);
	void executeCmd(Client *client, std::vector<std::string> const &res);
	static bool m_state;

	Client *clientExists(std::string nick);
	Channel *channelExists(std::string name);

private:
	std::string _creationTime;
	std::string _password;
	int _port;
	int _servfd;
	struct addrinfo *_res;
	std::vector<pollfd> _pfds;

	std::vector<Client *> _clients;
	std::vector<Channel *> _channels;

	bool _status;

	std::vector<std::string> _validCmds;
};
#endif
