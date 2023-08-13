/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 19:53:47 by abiru             #+#    #+#             */
/*   Updated: 2023/08/13 14:45:24 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

// enum MODES
// {
// 	AWAY,
// 	INVISIBLE,
// 	WALLOPS,
// };

class Client
{
	public:
		Client();
		~Client();

		void setFd(int fd);
		int getFd() const;

		void setNick(std::string const &nick);
		std::string const &getNick(void) const;
		
		void setUserName(std::string const &user);
		std::string const &getUserName() const;

		void setFullName(std::string const &fullName);
		std::string const &getFullName() const;

		void setJoinedTime(unsigned long time);
		unsigned long getJoinedTime() const;

		void setStatus(bool status);
		bool getStatus() const;

		void setPassword(bool value);
		bool hasPassword() const;
		
		void setMsgBuffer(std::string msg);
		std::string getMsgBuffer() const;

		void setIpAddr(struct sockaddr_storage *clientAddr);
		std::string const &getIpAddr() const;

	// msg format: USER username * * :full_name
	//             NICK nick
	private:
		std::string _nick;
		std::string _userName;
		std::string _fullName;
		bool _isRegistered;
		bool _password;
		int	_sockfd;
		std::string _msgBuffer;
		std::string _outGoingBuffer;
		char _msg[1024];
		unsigned long _joinedTime;
		std::string _ipAddr;
		// std::string msgBuffer;
};
#endif