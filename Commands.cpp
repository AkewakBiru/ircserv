/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youssef <youssef@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 11:55:48 by abiru             #+#    #+#             */
/*   Updated: 2023/09/23 22:41:11 by youssef          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

bool PASS(Server &server, Client *client, std::vector<std::string> const &res)
{
	if (!client)
		return (true);
	else if (client->getStatus())
		throw std::runtime_error(genErrMsg(ERR_ALREADYREGISTRED, client->getNick(), "", ERR_ALREADYREGISTRED_DESC));
	else if (res.size() == 2)
		throw std::invalid_argument(genErrMsg(ERR_NEEDMOREPARAMS, "*", res[1], ERR_NEEDMOREPARAMS_DESC));
	// set client's state to DOWN
	else if (res[2] != server.getPass())
	{
		client->setState(DOWN);
		throw std::invalid_argument(genServErrMsg(ERR_INCORRECT_PASSWORD, client->getIpAddr(), ERR_UNAUTHORIZED_ACCESS));
	}
	client->setPassword(true);
	return (true);
}

/*
	** sets nick for the user
		- if nick doesn't have any arg, sends ERR_NONICKNAMEGIVEN
		- if nick is already taken, sends ERR_NICKNAMEINUSE
		- if nick has more than 9 chars, sends ERR_TOOLONGNICKNAME
		- if nick has illegal chars, sends ERR_ERRONEUSNICKNAME
*/
bool NICK(Server &server, Client *client, std::vector<std::string> const &res)
{
	std::string tmp;
	if (client->getStatus())
		tmp = client->getNick();
	else
		tmp = "*";
	if (res.size() == 2)
		throw std::invalid_argument(genErrMsg(ERR_NONICKNAMEGIVEN, tmp, "", ERR_NONICKNAMEGIVEN_DESC));
	else if (hasIllegalChars(res[2]))
		throw std::invalid_argument(genErrMsg(ERR_ERRONEUSNICKNAME, tmp, res[2], ERR_ERRONEUSNICKNAME_DESC));
	else if (res[2].length() > 9)
		throw std::invalid_argument(genErrMsg(ERR_ERRONEUSNICKNAME, tmp, res[2], ERR_TOOLONGNICKNAME));
	else if (server.getNicksFd(res[2]) != -1 && server.getNicksFd(res[2]) != client->getFd())
		throw std::invalid_argument(genErrMsg(ERR_NICKNAMEINUSE, tmp, res[2], ERR_NICKNAMEINUSE_DESC));
	client->setNick(res[2]);
	return (true);
}

/*
	** sets username of the new user:
		- if user is already registered, sends ERR_ALREADYREGISTRED error msg
		- if user isn't registered, and command sent doesn't have enough params,
			sends ERR_NEEDMOREPARAMS error msg
*/
bool USER(Server &server, Client *client, std::vector<std::string> const &res)
{
	(void)server;
	if (client->getStatus())
		throw std::runtime_error(genErrMsg(ERR_ALREADYREGISTRED, client->getNick(), "", ERR_ALREADYREGISTRED_DESC));
	if (res.size() < 6)
		throw std::invalid_argument(genErrMsg(ERR_NEEDMOREPARAMS, "*", res[1], ERR_NEEDMOREPARAMS_DESC));
	else if (userIllegalChars(res[2]))
		throw std::invalid_argument(genErrMsg("", "*", res[1], ":user has illegal characters"));
	client->setUserName(res[2]);
	client->setFullName(res[5]);
	return (true);
}

static bool isValidOption(std::string const &arg)
{
	return (arg == "LS" || arg == "REQ" || arg == "ACK" || arg == "END" || arg == "LIST");
}

/*
 ** sends CAP negotiation reply to a client
 */
bool CAP(Server &server, Client *client, std::vector<std::string> const &res)
{
	(void)server;
	size_t resSize = res.size();

	if (resSize <= 2)
		throw std::invalid_argument(genErrMsg(ERR_NEEDMOREPARAMS, "*", res[1], ERR_NEEDMOREPARAMS_DESC));
	else if (!isValidOption(res[2]))
		throw std::invalid_argument(genErrMsg(ERR_INVALIDCAPCMD, "*", res[2], ERR_INVALIDCAPCMD_DESC));
	else if (res[2] == "LS" && (resSize == 3 || resSize == 4))
		sendMsg(client->getFd(), "CAP * LS :multi-prefix\r\n");
	// request to a change in capability is replied with an ACK
	// containing the list of capabilities that a client can do
	else if (res[2] == "REQ" && resSize >= 4)
		sendMsg(client->getFd(), "CAP * ACK :multi-prefix\r\n");
	return (true);
}

/*
 ** sends motd upon client authentication & request
 */
bool MOTD(Server &server, Client *client, std::vector<std::string> const &res)
{
	(void)server;
	std::ifstream file;
	std::stringstream buf;
	std::string strBuf;
	size_t end = 0;

	// for (std::vector<std::string>::const_iterator it = res.cbegin(); it != res.cend(); it++) {
	// 	std::cout << *it << " ";
	// }

	file.open("MOTD.conf", std::ios::in);
	if (!file.is_open())
		throw std::invalid_argument(genErrMsg(ERR_NOMOTD, "*", "*", ERR_NOMOTD_DESC));
	if (res.size() == 3 && res[2] != "ircserv")
	{
		file.close();
		throw std::invalid_argument(genErrMsg(ERR_NOSUCHSERVER, client->getNick(), res[2], ERR_NOSUCHSERVER_DESC));
	}
	buf << file.rdbuf();
	file.close();
	strBuf = buf.str();
	sendMsg(client->getFd(), genErrMsg(RPL_MOTDSTART, client->getNick(), "", RPL_MOTDSTART_DESC));
	while ((end = strBuf.find('\n')) != std::string::npos)
	{
		if (end > 80)
		{
			strBuf.erase(0, end + 1);
			continue;
		}
		sendMsg(client->getFd(), genErrMsg(RPL_MOTD, client->getNick(), ":-", strBuf.substr(0, end)));
		strBuf.erase(0, end + 1);
	}
	if (strBuf.size() && strBuf.size() <= 80)
		sendMsg(client->getFd(), genErrMsg(RPL_MOTD, client->getNick(), ":-", strBuf.substr(0, strBuf.size())));
	sendMsg(client->getFd(), genErrMsg(RPL_ENDOFMOTD, client->getNick(), "", RPL_ENDOFMOTD_DESC));
	return (true);
}

void	sendToRecipients(std::string formatedMessage, Client *client, Channel *channel) {
	if (client)
		client->setRecvMsgBuffer(formatedMessage);
	else {
		for (std::vector<Client *>::iterator it = channel->getMembers()->begin(); it != channel->getMembers()->end(); it++) {
			(*it)->setRecvMsgBuffer(formatedMessage);
		}
	}
}

bool PRIVMSG(Server &server, Client *client, std::vector<std::string> const &res)
{
	std::string recipientName;
	Client *recipientClient = NULL;
	Channel *recipientChannel = NULL;
	std::string message, formatedMessage;

	if (res.size() < 3)
		throw std::invalid_argument(genErrMsg(ERR_NORECIPIENT, "*", res[1], ERR_NORECIPIENT_DESC));
	if (res.size() < 4)
		throw std::invalid_argument(genErrMsg(ERR_NOTEXTTOSEND, "*", res[1], ERR_NOTEXTTOSEND_DESC));
	if (res[2].at(0) == '#')
	{
		for (std::vector<Channel *>::const_iterator it = server.getChannels().begin(); it != server.getChannels().end(); it++)
		{
			if (res[2].compare((*it)->getName()) == 0)
			{
				recipientChannel = *it;
				recipientName = (*it)->getName();
				break;
			}
		}
		if (!recipientChannel)
			throw std::invalid_argument(genErrMsg(ERR_CANNOTSENDTOCHAN, "*", res[1], ERR_CANNOTSENDTOCHAN_DESC));
	}
	else
	{
		for (std::vector<Client *>::const_iterator it = server.getClients().begin(); it != server.getClients().end(); it++) {
			if (res[2].compare((*it)->getNick()) == 0)
			{
				recipientClient = *it;
				recipientName = (*it)->getNick();
				break;
			}
		if (!recipientClient)
			throw std::invalid_argument(genErrMsg(ERR_NOSUCHNICK, "*", res[1], ERR_NOSUCHNICK_DESC));
		}
	}
	for (std::vector<std::string>::const_iterator it = res.cbegin() + 3; it != res.cend(); it++)
		message.append(*it);
	if (message.at(0) != ':')
		message.insert(1, ":");
	formatedMessage = ":" + client->getNick() + "! " + client->getUserName() + "@" + client->getIpAddr() + " privmsg " + recipientName + " " + message;
	sendToRecipients(formatedMessage, recipientClient, recipientChannel);
	return (true);
}

bool JOIN(Server &server, Client *client, std::vector<std::string> const &res)
{
	(void)server;
	(void)client;
	(void)res;
	return (true);
}

bool QUIT(Server &server, Client *client, std::vector<std::string> const &res)
{
	(void)server;
	(void)res;
	sendMsg(client->getFd(), genServErrMsg(client->getNick(), client->getIpAddr(), "Quit: " + client->getNick()));
	client->setState(DOWN);
	return (true);
}
