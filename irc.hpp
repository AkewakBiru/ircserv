/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 21:54:23 by abiru             #+#    #+#             */
/*   Updated: 2023/07/27 16:25:36 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP
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
		bool listenConn(void) const;

	private:
		std::string _password;
		int _port;
		int _servfd;
		struct addrinfo *_res;
};
#endif
