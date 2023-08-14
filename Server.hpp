/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 21:54:23 by abiru             #+#    #+#             */
/*   Updated: 2023/08/15 00:02:04 by abiru            ###   ########.fr       */
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
#include "responseCodes.hpp"

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

		bool deleteConnection(int fd);

		void addClient(Client *client);
		void removeClient(Client *client);
		bool isRegistered(int fd);
		bool registerUser(int fd, std::vector<std::string> const &res, char const *msg, size_t index);
		std::vector<Client *>::iterator findFd(std::vector<Client *>& client, int fd);
		ssize_t getNicksFd(std::string nick);

		bool sendMsgAndCloseConnection(std::string const &msg, size_t index);
		void sendWelcomingMsg(Client *client);

		void setStatus(bool status);
		int getStatus() const;

	private:
		std::string _creationTime;
		std::string _password;
		int _port;
		int _servfd;
		struct addrinfo *_res;
		int _fdCount;
		std::vector<pollfd> _pfds;

		std::vector<Client *> _clients;
		std::vector<Channel *> _channels;
		
		bool _status;
};
#endif
