/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 21:54:23 by abiru             #+#    #+#             */
/*   Updated: 2023/07/28 14:23:29 by abiru            ###   ########.fr       */
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
		void setPass(std::string &newPass);
		int getPort() const;
		std::string const &getPass() const;
		void checkParams() const;
		void setServFd(int const fd);
		int getServFd() const;

		bool getServAddrInfo(void);
		bool createSocket(void);
		bool listenForConn(void) const;

		void addClient(Client &client);
		void removeClient(Client &client);
		bool isRegistered(int fd);
		void registerUser(int fd, std::vector<std::string> const &res);
		std::vector<Client>::iterator findFd(std::vector<Client>& client, int fd);

	private:
		std::string _password;
		int _port;
		int _servfd;
		struct addrinfo *_res;

		std::vector<Client> _clients;
		std::vector<Channel*> _channels;
};
#endif
