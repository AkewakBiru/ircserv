/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 19:53:49 by abiru             #+#    #+#             */
/*   Updated: 2023/08/09 22:49:46 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(): _nick(""), _userName(""), _fullName(""), _isRegistered(false), _sockfd(-1), _buffer(""), _joinedTime(0)
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