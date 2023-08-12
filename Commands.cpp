/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 11:55:48 by abiru             #+#    #+#             */
/*   Updated: 2023/08/13 01:01:10 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

bool PASS(ServParams const &server, Client *client, std::vector<std::string> const &res)
{
	if (!client)
		return (true);
	if (client->getStatus())
		throw std::runtime_error(genErrMsg(ERR_ALREADYREGISTRED, client->getNick(), "", ERR_ALREADYREGISTRED_DESC));
	if (res.size() == 2)
		throw std::invalid_argument(genErrMsg(ERR_NEEDMOREPARAMS, "*", res[1], ERR_NEEDMOREPARAMS_DESC));
	if (!client->hasPassword() && (res.size() != 3 || res[2] != server.getPass()))
		return (true);
	else if (res[2] == server.getPass())
		client->setPassword(true);
	return (true);
}

bool NICK(ServParams &server, Client *client, std::vector<std::string> const &res)
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
}
