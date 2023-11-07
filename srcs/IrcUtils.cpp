/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 20:51:50 by abiru             #+#    #+#             */
/*   Updated: 2023/11/07 22:59:12 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IrcUtils.hpp"

bool hasWhiteSpace(std::string const &str)
{
	return (str.find_first_of(" \t\r\n\v\f") != std::string::npos);
}

/*
 ** checks illegal Characters on a nickname
 ** illegal chars: A digit or '-' at 0 index
 ** non-digit, non-letter, or non (%x5B-60 / %x7B-7D) char at any index
 */
bool hasIllegalChars(std::string const &arg)
{
	if (arg.length() > 0 && (std::isdigit(arg[0]) || arg[0] == '-'))
		return (true);
	for (size_t i = 0; i < arg.length(); i++)
	{
		if (!std::isdigit(arg[i]) && (arg[i] < 'A' || arg[i] > 125) && arg[i] != '-')
			return (true);
	}
	return (false);
}

/*
 ** checks if a username has one of ("\0\r\n@\t") in it
 */
bool userIllegalChars(std::string const &arg)
{
	return (arg.find_first_of("\0\r\n@\t") != std::string::npos);
}

/*
 ** returns the upperCase equivalent of a string's alpha chars
 ** if flag is set, changes the case of a few special chars as
 ** per RFC 2812 (for nickname)
 */
std::string toUpper(std::string const &arg, bool flag)
{
	std::string newStr(arg);
	if (arg.size() == 0)
		return ("");
	for (size_t i = 0; i < newStr.length(); i++)
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
	if (sig == 13)
		return;
	Server::m_state = STOPPED;
	std::cout << GREEN << "\n:ircserv 10.0 has stopped gracefully" << RESET << std::endl;
}

void signalHandler(void)
{
	struct sigaction sa;
	sa.sa_flags = 0;
	sa.sa_handler = sigHandle;

	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, 0);
	sigaction(SIGQUIT, &sa, 0);
	sigaction(SIGPIPE, &sa, 0);
}

void sendMsg(int fd, std::string msg)
{
	if (send(fd, msg.c_str(), msg.length(), 0) == -1)
		printError("send: ", strerror(errno));
}

bool isSpaces(std::string const &str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] != ' ' && str[i] != '\n' && str[i] != '\r')
			return (false);
		else if ((str[i] == '\n' && i == 0) || (str[i] == '\n' && str[i - 1] != '\r'))
			return (false);
	}
	return (true);
}

/*
 ** right trims a substring
 */
std::string &rtrim(std::string &str, std::string const &set)
{
	if (ssize_t start = str.rfind(set) != std::string::npos)
		str.erase(str.length() - start - 1, set.length());
	return (str);
}

bool isValidCmd(std::string const &cmd, std::vector<std::string> const &cmdList)
{
	std::vector<std::string>::const_iterator it = (std::find(cmdList.begin(), cmdList.end(), cmd));
	return (it != cmdList.end());
}

/*
 ** empty msg (msg with only one of or a combination of " \r\n\v\f\0" )
 ** and msg with bad char ('\0') in the middle of it gets dropped
 */
bool preParseInput(Client *client, char const *msg, ssize_t size)
{
	bool flag = true;
	for (ssize_t i = 0; i < size; i++)
	{
		if (msg[i] != '\r' && msg[i] != '\n' && msg[i] != '\0' && msg[i] != '\f' && msg[i] != '\v')
		{
			flag = true;
			break;
		}
		flag = false;
	}
	if (!flag)
		return (false);
	if (std::strchr(msg, '\0') && std::strchr(msg, '\0') - msg < size)
	{
		client->setRecvMsgBuffer(genErrMsg(ERR_UNKNOWNCOMMAND, "*", "*", ERR_UNKNOWNCOMMAND_ILLEGAL));
		return (false);
	}
	return (true);
}

bool checkPort(std::string arg)
{
	size_t i = 0;
	while (i < arg.length() && std::isspace(arg[i]))
		i++;
	if (i < arg.length() && arg[i] == '+')
		i++;
	return (arg.find_first_not_of("0123456789", i) == std::string::npos);
}

/*
 ** Sends formattedMessage to either client or channel.
 ** The formatedMessage will be sent to all clients of the channel
 ** whose file descriptors do not match fd.
 */
void sendToRecipients(std::string formatedMessage, Client *client, Channel *channel, int fd)
{
	if (client)
		client->setRecvMsgBuffer(formatedMessage);
	else
		for (std::vector<Client *>::iterator it = channel->getMembers()->begin(); it != channel->getMembers()->end(); it++)
			if (fd != (*it)->getFd())
				(*it)->setRecvMsgBuffer(formatedMessage);
}

void printError(std::string command, std::string msg)
{
	std::cerr << RED << command << msg << RESET << std::endl;
}
void logClientStatus(std::string ip, int fd, bool leaving)
{
	std::stringstream msg;

	if (leaving)
	{
		msg << "Lost connection to " << ip + " on socket " << fd;
		std::cout << RED << msg.str() << RESET << std::endl;
	}
	else
	{
		msg << "Received connection from " << ip << " on socket: " << fd;
		std::cout << GREEN << msg.str() << RESET << std::endl;
	}
}
void printCommand(std::string msg)
{
	std::cout << PURPLE
			  << "\nINSTRUCTION\n"
			  << "-----------------\n"
			  << msg
			  << RESET << std::endl;
}