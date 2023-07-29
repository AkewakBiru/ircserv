/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 21:54:23 by abiru             #+#    #+#             */
/*   Updated: 2023/07/29 16:11:17 by abiru            ###   ########.fr       */
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
#include "Parser.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <algorithm>

class ServParams
{
	public:
		ServParams(std::string pass, int const port);
		~ServParams();
		class InvalidPortException: public std::exception
		{
			const char *what () const throw();
		};
		class InvalidPasswordException: public std::exception
		{
			const char *what () const throw();
		};
		void setPass(std::string &newPass);
		int getPort() const;
		std::string const &getPass() const;
		void checkParams() const;
		void setServFd(int const fd);
		int getServFd() const;

		bool getServAddrInfo(void);
		bool createSocket(void);
		bool listenForConn(void) const;
		bool handleRequest(void);

		void addNewConn(int fd);
		void removeConn(int index);

		void addClient(Client &client);
		void removeClient(Client &client);
		bool isRegistered(int fd);
		bool registerUser(int fd, std::vector<std::string> const &res);
		std::vector<Client>::iterator findFd(std::vector<Client>& client, int fd);

	private:
		std::string _password;
		int _port;
		int _servfd;
		struct addrinfo *_res;
		int _fdCount;
		struct pollfd* _pfds;

		std::vector<Client> _clients;
		std::vector<Channel*> _channels;
};
#endif
