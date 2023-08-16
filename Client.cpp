/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 19:53:49 by abiru             #+#    #+#             */
/*   Updated: 2023/08/16 16:44:33 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(): _nick(""), _userName(""), _fullName(""), _isRegistered(false), _password(false), \
_sockfd(-1), _outGoingBuffer(""), _joinedTime(0), _ipAddr(""), _recvBuf("")
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

/*
	** adds recvd msg to _recvBuf & finds part of the msg that has CRLF,
	** puts it in _execBuf.
*/
void Client::addToBuffer(std::string msg)
{
	size_t pos = 0;
	
	// std::cout << msg << "\n";
	if (isSpaces(msg))
		return ;
	_recvBuf.append(msg);
	while ( (pos = _recvBuf.find("\r\n")) != std::string::npos )
	{
		_execBuf.push(_recvBuf.substr(0, pos + 2));
		_recvBuf.erase(0, pos + 2);
	}
	std::cout << "recved buffer: " << _recvBuf << "\n";
	// for (std::vector<std::string>::iterator it = _dataBuffer.begin(); it!=_dataBuffer.end(); ++it)
	// 	std::cout << "[" << *it << "]";
}

std::queue<std::string> const &Client::getDataBuffer() const
{
	return (_execBuf);
}

void Client::rmvfromBuf()
{
	_execBuf.pop();
}