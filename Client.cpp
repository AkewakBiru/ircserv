/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 19:53:49 by abiru             #+#    #+#             */
/*   Updated: 2023/07/28 14:31:11 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(): _nick(""), _userName(""), _fullName(""), _isRegistered(false), _sockfd(-1)
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