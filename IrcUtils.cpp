/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 20:51:50 by abiru             #+#    #+#             */
/*   Updated: 2023/08/15 20:42:05 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcUtils.hpp"

bool hasWhiteSpace(std::string const &str)
{
	for (size_t i=0; i<str.length(); i++)
	{
		if (std::isspace(str[i]))
			return (true);
	}
	return (false);
}

/*
	** illegal chars -> A digit at 0 index
	** 
*/
bool hasIllegalChars(std::string const &arg)
{
	if (arg.length() > 0 && (std::isdigit(arg[0]) || arg[0] == '-'))
		return (true);
	for (size_t i=0; i<arg.length(); i++)
	{
		if ((arg[i] < 'A' || arg[i] > 125) && arg[i] != '-')
			return (true);
	}
	return (false);
}

bool userIllegalChars(std::string const &arg)
{
	const char *options = "\0\r\n@ ";
	for (size_t i=0; i<arg.length(); i++)
	{
		for (uint8_t j=0; j<5; j++)
		{
			if (arg[i] == options[j])
				return (true);
		}
	}
	return (false);
}

std::string toUpper(std::string const &arg, bool flag)
{
	std::string newStr(arg);
	if (arg.size() == 0)
		return ("");
	for (size_t i=0; i<newStr.length(); i++)
	{
		if (newStr[i] >= 'a' && newStr[i] <= 'z')
			newStr[i] = newStr[i] - ' ';
		else if (flag)
		{
			if (newStr[i] == '{')
				newStr[i] = '[';
			else if (newStr[i] == '}')
				newStr[i] = ']';
			else if (newStr[i] == '|')
				newStr[i] = '\\';
			else if (newStr[i] == '^')
				newStr[i] = '~';
		}
	}
	return (newStr);
}

std::string genErrMsg(char const *errCode, std::string optional, std::string cmd, std::string desc)
{
	std::string error = ":ircserv ";
	error.append(errCode).append(" ");
	if (optional.length() > 0)
		error.append(optional).append(" ");
	if (cmd.length() > 0)
		error.append(cmd).append(" ");
	error.append(desc).append("\r\n");
	return (error);
}

std::string genServErrMsg(std::string nick, std::string ip, std::string desc)
{
	std::string error = "ERROR :Closing Link: ";
	error.append(nick).append("[").append(ip).append("]").append(" (").append(desc).append(")\r\n");
	return (error);
}

Server *getServerInstance(Server *instance)
{
	static Server *ptr;

	if (instance)
		ptr = instance;
	return (ptr);
}

void sigHandle(int sig)
{
	(void) sig;
	
	getServerInstance(0)->setStatus(STOPPED);
	std::cout << "\n:ircserv 10.0 has stopped" << std::endl;
}

void sendMsg(int fd, std::string msg)
{
	send(fd, msg.c_str(), msg.length(), 0);
}