/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 19:53:49 by abiru             #+#    #+#             */
/*   Updated: 2023/11/07 22:59:14 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client() : _nick(""), _userName(""), _fullName(""), _isRegistered(false), _password(false),
				   _sockfd(-1), _joinedTime(0), _ipAddr(""), _recvBuf(""), _state(UP), _timeOutMsgSent(false)
{
}

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

void Client::setState(bool state)
{
	_state = state;
}

bool Client::getState() const
{
	return (_state);
}

void Client::setRecvMsgBuffer(std::string msg)
{
	if (msg.size() > 510)
	{
		while (msg.size())
		{
			if (msg.size() > 510)
			{
				_recvMsgBuffer.push(msg.substr(0, 510));
				msg.erase(0, 510);
			}
			else if (msg.size())
			{
				_recvMsgBuffer.push(msg.substr(0, msg.size()));
				msg.erase(0, msg.size());
			}
		}
	}
	else
		_recvMsgBuffer.push(msg);
}

std::queue<std::string> Client::getRecvMsgBuffer() const
{
	return (_recvMsgBuffer);
}

void Client::rmvfromRcvBuf()
{
	_recvMsgBuffer.pop();
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
	if (((struct sockaddr *)clientAddr)->sa_family == AF_INET)
		_ipAddr = inet_ntoa(((struct sockaddr_in *)clientAddr)->sin_addr);
	else
		_ipAddr = inet_ntoa(((struct sockaddr_in *)clientAddr)->sin_addr);
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
	std::string disallowed = " \r\n\v\f\0";

	if (isSpaces(msg))
		return;
	_recvBuf.append(msg);
	while ((pos = _recvBuf.find("\r\n")) != std::string::npos)
	{
		_execBuf.push(_recvBuf.substr(0, pos + 2));
		_recvBuf.erase(0, pos + 2);
	}
}

std::queue<std::string> const &Client::getDataBuffer() const
{
	return (_execBuf);
}

void Client::rmvfromBuf()
{
	_execBuf.pop();
}

void Client::setTimeOutMsgSent(bool isSent)
{
	_timeOutMsgSent = isSent;
}

bool Client::getTimeOutMsgSent() const
{
	return (_timeOutMsgSent);
}

std::string Client::getChanList(Server &server)
{
	std::string chans = "";
	for (std::vector<Channel *>::const_iterator it = server.getChannels().begin(); it != server.getChannels().end(); it++)
		if ((*it)->isMember(this))
			chans.append((*it)->getName() + " ");
	return (chans);
}
