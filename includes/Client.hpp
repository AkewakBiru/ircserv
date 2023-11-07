/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 19:53:47 by abiru             #+#    #+#             */
/*   Updated: 2023/10/27 15:54:13 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <string>
#include <queue>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include "IrcUtils.hpp"
#include "Channel.hpp"
#include "Server.hpp"

class Server;

enum STATUS
{
	DOWN,
	UP
};

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

	void setState(bool state);
	bool getState() const;

	void setRecvMsgBuffer(std::string msg);
	std::queue<std::string> getRecvMsgBuffer() const;

	void rmvfromRcvBuf();

	void setMsgDest(std::string dest);
	std::string &getMsgDest();

	void setIpAddr(struct sockaddr_storage *clientAddr);
	std::string const &getIpAddr() const;

	void addToBuffer(std::string msg);
	void rmvfromBuf();
	std::queue<std::string> const &getDataBuffer() const;

	void setTimeOutMsgSent(bool isSent);
	bool getTimeOutMsgSent() const;

	std::string getChanList(Server &server);

private:
	std::string _nick;
	std::string _userName;
	std::string _fullName;
	bool _isRegistered;
	bool _password;
	int _sockfd;
	std::string _dest;
	unsigned long _joinedTime;
	std::string _ipAddr;

	std::string _recvBuf;
	std::queue<std::string> _execBuf;
	std::queue<std::string> _recvMsgBuffer;

	bool _state;

	bool _timeOutMsgSent;
};
#endif