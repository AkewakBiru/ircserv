/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 19:53:49 by abiru             #+#    #+#             */
/*   Updated: 2023/08/15 23:21:04 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(): _nick(""), _userName(""), _fullName(""), _isRegistered(false), _password(false), _sockfd(-1), _dataBuffer(0), _outGoingBuffer(""), _joinedTime(0), _ipAddr("")
{}

Client::~Client()
{
	if (_sockfd >= 0)
		close(_sockfd);
}

void Client::setFd(int fd)
{
	_sockfd = fd;
}

int Client::getFd() const
{
	return (_sockfd);
}

void Client::setNick(std::string const &nick)
{
	_nick = nick;
}

std::string const &Client::getNick(void) const
{
	return (_nick);
}

void Client::setUserName(std::string const &user)
{
	_userName = user;
}

std::string const &Client::getUserName() const
{
	return (_userName);
}

void Client::setFullName(std::string const &fullName)
{
	_fullName = fullName;
}

std::string const &Client::getFullName() const
{
	return (_fullName);
}
		
void Client::setJoinedTime(unsigned long time)
{
	_joinedTime = time;
}

unsigned long Client::getJoinedTime() const
{
	return (_joinedTime);
}

void Client::setStatus(bool status)
{
	_isRegistered = status;
}

bool Client::getStatus() const
{
	return (_isRegistered);
}

void Client::setPassword(bool value)
{
	_password = value;
}

bool Client::hasPassword() const
{
	return (_password);
}

// void Client::setMsgBuffer(std::string msg)
// {
// 	_msgBuffer = msg;
// }
// std::string &Client::getMsgBuffer()
// {
// 	return (_msgBuffer);
// }

void Client::setOutgoingMsgBuffer(std::string msg)
{
	_outGoingBuffer = msg;
}

std::string &Client::getOutgoingMsgBuffer()
{
	return (_outGoingBuffer);
}

void Client::setMsgDest(std::string dest)
{
	_dest = dest;
}

std::string &Client::getMsgDest()
{
	return (_dest);
}

void Client::setIpAddr(struct sockaddr_storage *clientAddr)
{
	void *addr;
	char ip[INET6_ADDRSTRLEN];

	if (((struct sockaddr *)clientAddr)->sa_family == AF_INET)
		addr = &((struct sockaddr_in *)clientAddr)->sin_addr;
	else
		addr = &((struct sockaddr_in6 *)clientAddr)->sin6_addr;
	_ipAddr = inet_ntop(clientAddr->ss_family, addr, ip, INET6_ADDRSTRLEN);
}

std::string const &Client::getIpAddr() const
{
	return (_ipAddr);
}

void Client::addToBuffer(std::string msg)
{
	size_t start = 0, end = 0;
	
	while (start < msg.length())
	{
		end = msg.find("\r\n", start);
		if (end != std::string::npos)
			_dataBuffer.push_back(msg.substr(start, end - start + 2));
		else
		{
			_dataBuffer.push_back(msg.substr(start, msg.length() - start + 1));
			break ;
		}
		start = end + 2;
	}
	// for (std::vector<std::string>::iterator it = _dataBuffer.begin(); it!=_dataBuffer.end(); ++it)
	// 	std::cout << "[" << *it << "]";
}