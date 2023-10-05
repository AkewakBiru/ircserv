/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youssef <youssef@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 11:55:48 by abiru             #+#    #+#             */
/*   Updated: 2023/10/05 18:36:04 by youssef          ###   ########.fr       */
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
		throw std::invalid_argument(genErrMsg(ERR_INCORRECT_PASSWORD, "", client->getIpAddr(), ERR_UNAUTHORIZED_ACCESS));
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
	if (res[2] == client->getNick())
		return (false);
	client->setNick(res[2]);
	if (client->getStatus())
		client->setRecvMsgBuffer(":nick NICK :" + res[2] + "\r\n");
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
		client->setRecvMsgBuffer("CAP * LS :multi-prefix\r\n");
	// request to a change in capability is replied with an ACK
	// containing the list of capabilities that a client can do
	else if (res[2] == "REQ" && resSize >= 4)
		client->setRecvMsgBuffer("CAP * ACK :multi-prefix\r\n");
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
	client->setRecvMsgBuffer(genErrMsg(RPL_MOTDSTART, client->getNick(), "", RPL_MOTDSTART_DESC));
	while ((end = strBuf.find('\n')) != std::string::npos)
	{
		if (end > 80)
		{
			strBuf.erase(0, end + 1);
			continue;
		}
		client->setRecvMsgBuffer(genErrMsg(RPL_MOTD, client->getNick(), ":-", strBuf.substr(0, end)));
		strBuf.erase(0, end + 1);
	}
	if (strBuf.size() && strBuf.size() <= 80)
		client->setRecvMsgBuffer(genErrMsg(RPL_MOTD, client->getNick(), ":-", strBuf.substr(0, strBuf.size())));
	client->setRecvMsgBuffer(genErrMsg(RPL_ENDOFMOTD, client->getNick(), "", RPL_ENDOFMOTD_DESC));
	return (true);
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
		recipientChannel = server.channelExists(res[2]);
		if (!recipientChannel)
			throw std::invalid_argument(genErrMsg(ERR_CANNOTSENDTOCHAN, "*", res[1], ERR_CANNOTSENDTOCHAN_DESC));
		recipientName = recipientChannel->getName();
	}
	else
	{
		recipientClient = server.clientExists(res[2]);
		if (!recipientClient)
			throw std::invalid_argument(genErrMsg(ERR_NOSUCHNICK, "*", res[1], ERR_NOSUCHNICK_DESC));
		recipientName = recipientClient->getNick();
	}
	for (std::vector<std::string>::const_iterator it = res.begin() + 3; it != res.end(); it++)
		message.append(*it + " ");
	if (message.at(0) != ':')
		message.insert(0, ":");
	formatedMessage = ":" + client->getNick() + "!" + client->getUserName() + "@" + client->getIpAddr() + " PRIVMSG " + recipientName + " " + message + "\r\n";
	sendToRecipients(formatedMessage, recipientClient, recipientChannel, -1);
	return (true);
}

bool JOIN(Server &server, Client *client, std::vector<std::string> const &res)
{
	Channel *channel = NULL;
	std::string password;

	if (res.size() < 3)
		throw std::invalid_argument(genErrMsg(ERR_NEEDMOREPARAMS, "*", res[1], ERR_NEEDMOREPARAMS_DESC));
	if (res[2].compare("0") == 0)
		// exit all channels using part
	if (res[2].at(0) != '#')
			throw std::invalid_argument(genErrMsg(ERR_INVALIDCHANNAME, "*", res[1], ERR_INVALIDCHANNAME_DESC));
	if (res.size() >= 4)
		password = res[3];
	channel = server.channelExists(res[2]);
	if (!channel)
	{
		channel = new Channel(res[2]);
		server.addChannel(channel);
		// add user to operators
	}
	if (channel->getMode('i') && !channel->isInvited(client))
		throw std::invalid_argument(genErrMsg(ERR_INVITEONLYCHAN, "*", res[1], ERR_INVITEONLYCHAN_DESC));
	if (channel->getMembers()->size() >= static_cast<unsigned long>(channel->getMaxUsers()))
		throw std::invalid_argument(genErrMsg(ERR_CHANNELISFULL, "*", res[1], ERR_CHANNELISFULL_DESC));
	if (channel->getMode('k') && (res.size() < 4 || channel->getPassword().compare(password)))
		throw std::invalid_argument(genErrMsg(ERR_BADCHANNELKEY, "*", res[1], ERR_BADCHANNELKEY_DESC));
	std::vector<Client *>::iterator iter = std::find(channel->getMembers()->begin(), channel->getMembers()->end(), client);
	if (iter != channel->getMembers()->end())
		return (false);
	channel->addUser(client);
	// send a message to all members that a new client has joined
	sendToRecipients(":" + client->getNick() + "!" + client->getUserName() + "@" + client->getIpAddr() + " JOIN :" + res[2] + "\r\n", NULL, channel, -1);
	NAMES(server, client, channel);
	return (true);
}

bool QUIT(Server &server, Client *client, std::vector<std::string> const &res)
{
	std::string quitMsg = ":" + client->getNick() + "!" + client->getUserName() + "@ircserv QUIT";
	if (res.size() > 2)
		quitMsg += " :";
	for (size_t i = 2; i < res.size(); i++)
	{
		if (i == 2 && res[2].at(0) == ':')
			quitMsg += res[i].substr(1) + " ";
		else
			quitMsg += res[i] + " ";
	}
	quitMsg += "\r\n";
	std::cout << quitMsg;
	for (std::vector<Channel *>::const_iterator it = server.getChannels().begin(); it != server.getChannels().end(); it++)
		if ((*it)->isMember(client))
			sendToRecipients(quitMsg, NULL, *it, client->getFd());
	client->setState(DOWN);
	return (true);
}

/*
 * when a client joins, info about the channel is sent to the client
 */
bool NAMES(Server &server, Client *client, Channel *channel)
{
	(void)server;
	std::string message = ":ircserv 353 " + client->getNick() + " = " + channel->getName() + " :";
	std::vector<Client *> *members = channel->getMembers();
	std::string chanop = "";
	for (std::vector<Client *>::iterator it = members->begin(); it != members->end(); it++)
	{
		if ((*it)->isOperator(channel))
			chanop = (*it)->getNick();
		else
			message += (*it)->getNick() + " ";
	}
	client->setRecvMsgBuffer(message + "@" + chanop + "\r\n");
	client->setRecvMsgBuffer(":ircserv 366 " + client->getNick() + " " + channel->getName() + " :End of /NAMES list.\r\n");
	return (true);
}

bool INVITE(Server &server, Client *client, std::vector<std::string> const &res)
{
	Client *invitee;
	Channel *channel;

	if (res.size() < 4)
		throw std::invalid_argument(genErrMsg(ERR_NEEDMOREPARAMS, "*", res[1], ERR_NEEDMOREPARAMS_DESC));
	// check if client exists
	invitee = server.clientExists(res[2]);
	if (!invitee)
		throw std::invalid_argument(genErrMsg(ERR_NOSUCHNICK, "nick " + client->getNick(), res[1], ERR_NOSUCHNICK_DESC));
	// check if channel exists
	channel = server.channelExists(res[3]);
	if (!channel)
		throw std::invalid_argument(genErrMsg(ERR_NOSUCHCHANNEL, client->getNick(), res[1], ERR_NOSUCHCHANNEL_DESC));
	//check if this client is an operator
	if (!client->isOperator(channel))
		throw std::invalid_argument(genErrMsg(ERR_CHANOPRIVSNEEDED, "*", res[1], ERR_CHANOPRIVSNEEDED_DESC));
	std::vector<Client *>::const_iterator it3 = std::find(channel->getMembers()->begin(), channel->getMembers()->end(), invitee);
	if (it3 != channel->getMembers()->end())
		throw std::invalid_argument(genErrMsg(ERR_USERSDISABLED, "nick " + client->getNick(), res[1], ERR_USERSDISABLED_DESC));
	channel->addUser(invitee);
	// send a message to all members that a new client has joined
	sendToRecipients(":" + invitee->getNick() + "!" + invitee->getUserName() + "@" + invitee->getIpAddr() + " INVITE :" + res[3] + "\r\n", NULL, channel, -1);
	NAMES(server, invitee, channel);
	return (true);
}

void KICK(Server &server, Client *client, std::vector<std::string> const &res)
{
	(void)server;
	// Check if enough arguments are provided
	if (res.size() < 3)
		throw std::invalid_argument(genErrMsg(ERR_NEEDMOREPARAMS, "*", res[1], ERR_NEEDMOREPARAMS_DESC));

	// Get the channel name and nickname of the user to be kicked
	std::string channelName = res[1];
	std::string nickToKick = res[2];

	// Retrieve the channel object
	Channel *channel = Channel::getChannel(channelName);
	if (channel == NULL)
	{
		throw std::invalid_argument(genErrMsg(ERR_NOSUCHCHANNEL, "*", channelName, "No such channel"));
	}

	// Check if the client is actually on the channel
	if (!channel->isMember(client))
		throw std::invalid_argument(genErrMsg(ERR_NOTONCHANNEL, "*", channelName, "You're not on that channel"));

	// Check if the client is an operator in the channel
	if (!client->isOperator(channel))
	{
		throw std::invalid_argument(genErrMsg(ERR_CHANOPRIVSNEEDED, "*", channelName, "You're not channel operator"));
	}

	// Declare and initialize userFound to false
	bool userFound = false;

	// Loop through the channel members to find the user to kick
	std::vector<Client *> *users = channel->getMembers();
	for (std::vector<Client *>::iterator it = users->begin(); it != users->end(); ++it)
	{
		Client *user = *it;
		if (client->getNick() == nickToKick)
		{
			// Notify the channel that the user has been kicked
			sendToRecipients(client->getNick() + " has kicked " + nickToKick + " from " + channel->getName() + "!", NULL, channel, client->getFd());

			// Notify the kicked user
			sendToRecipients("You have been kicked from the channel.", user, NULL, 0);

			// Remove the user from the channel
			channel->removeUser(user);
			// Set userFound to true and break the loop
			userFound = true;
			break;
		}
	}

	if (!userFound)
	{
		throw std::invalid_argument(genErrMsg(ERR_USERNOTINCHANNEL, "*", nickToKick, "They aren't on that channel"));
	}
}

void setTopic(Server &server, Client *client, std::vector<std::string> const &res)
{
	(void)server;
	// Validate the input messages
	if (res.size() < 3)
		throw std::invalid_argument(genErrMsg(ERR_NEEDMOREPARAMS, "*", res[1], ERR_NEEDMOREPARAMS_DESC));

	// Get the channel name and find the channel object
	std::string channelName = res[1];
	Channel *channel = Channel::getChannel(channelName);

	if (!channel)
		throw std::invalid_argument(genErrMsg(ERR_NOSUCHCHANNEL, "*", channelName, ERR_NOSUCHCHANNEL_DESC));

	// Check if the client is a member of the channel
	if (!channel->isMember(client))
		throw std::invalid_argument(genErrMsg(ERR_NOTONCHANNEL, "*", channelName, ERR_NOTONCHANNEL_DESC));

	// Check if the client is an operator and if the topic is operator-only
	if (channel->getMode('t') && !client->isOperator(channel))
		throw std::invalid_argument(genErrMsg(ERR_CHANOPRIVSNEEDED, "*", channelName, ERR_CHANOPRIVSNEEDED));

	// Extract the new topic from the messages
	unsigned int startIndex = (res[2] == ":") ? 3 : 2;
	std::string topic = "";

	if (res.size() > 3 || res[2] != ":")
	{
		topic = res[startIndex];
		for (unsigned int i = startIndex + 1; i < res.size(); i++)
		{
			topic += " " + res[i];
		}
	}

	// Set the new topic
	channel->setTopic(topic);
	// send a confirmation message
	sendToRecipients("Topic for channel " + channel->getName() + " has been set to: " + channel->getTopic(), client, NULL, 0);
}

void manageMods(Server &server, Client *client, std::vector<std::string> const &res)
{
	(void)server;
	// Check for minimum required parameters
	if (res.size() < 3)
		throw std::invalid_argument(genErrMsg(ERR_NEEDMOREPARAMS, "*", res[1], ERR_NEEDMOREPARAMS_DESC));

	// Initialize a boolean to keep track of the mode being set (+) or unset (-)
	bool mode_bool = false;

	// Get the channel object
	Channel *channel = Channel::getChannel(res[1]);

	// Check if the channel exists
	if (!channel)
		throw std::invalid_argument(genErrMsg(ERR_NOSUCHCHANNEL, "*", res[1], "No such channel"));

	// Check if the client is an operator
	if (!client->isOperator(channel))
		throw std::invalid_argument(genErrMsg(ERR_CHANOPRIVSNEEDED, "*", res[1], "You're not channel operator"));

	// Extract the mode string from the messages
	std::string mode = res[2];
	std::string mode_str = "";

	// Validate the mode string
	if (mode.length() < 2 || (mode[0] != '+' && mode[0] != '-'))
		throw std::invalid_argument(genErrMsg(ERR_UNKNOWNMODE, "*", res[1], "Unknown mode"));

	// Loop through the mode string to set or unset each mode
	for (unsigned int i = 0; i < mode.length(); i++)
	{
		// Handle each mode
		if (mode[i] == '+')
		{
			mode_bool = true;
			mode_str += "+";
		}
		else if (mode[i] == '-')
		{
			mode_bool = false;
			mode_str += "-";
		}
		else if (mode[i] == 'o' && res.size() > 3)
		{
			std::string nick = res[3];
			// Loop through all clients to find the one with the matching nickname
			std::vector<Client *> *members = channel->getMembers();
			for (std::vector<Client *>::iterator it = members->begin(); it != members->end(); ++it)
			{
				Client *user = *it;
				if (user->getNick() == nick)
				{
					channel->setOperatorStatus(user, mode_bool);
					if (user->isOperator(channel))
						break;
				}
			}
			// Update the channel's mode
			channel->setMode('o', mode_bool);
			mode_str += "o";
		}
		else if (mode[i] == 'i')
		{
			channel->setMode('i', mode_bool);
			mode_str += "i";
		}
		else if (mode[i] == 't')
		{
			channel->setMode('t', mode_bool);
			mode_str += "t";
		}
		else if (mode[i] == 'l' && res.size() > 3)
		{
			// Check if the limit is within a valid range
			if (mode_bool && std::atoi(res[3].c_str()) > 0 && std::atoi(res[3].c_str()) < 1000)
			{
				channel->setMaxUsers(std::atoi(res[3].c_str()));
				channel->setMode('l', true);
			}
			else
			{
				channel->setMode('l', false);
				channel->setMaxUsers(1000);
			}
			mode_str += "l";
		}
		else if (mode[i] == 'k' && res.size() > 3)
		{
			// Check if a password is provided
			if (mode_bool && !res[3].empty())
			{
				channel->setMode('k', true);
				channel->setPassword(res[3]);
			}
			else
			{
				channel->setMode('k', false);
				channel->setPassword("");
			}
			mode_str += "k";
		}
		else
			return;
	}
	// Send a message to the channel indicating the mode change
	sendToRecipients(":" + client->getNick() + " MODE " + channel->getName() + " " + mode_str + "\r\n", NULL, channel, -1);
}

bool PING(Server &server, Client *client, std::vector<std::string> const &res)
{
	(void)server;
	if (res.size() > 2)
		throw std::invalid_argument(genErrMsg(ERR_NOSUCHSERVER, client->getNick(), res[2], ERR_NOSUCHSERVER_DESC));
	client->setRecvMsgBuffer(":ircserv PONG\r\n");
	return (true);
}
