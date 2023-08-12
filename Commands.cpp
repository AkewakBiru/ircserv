/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 11:55:48 by abiru             #+#    #+#             */
/*   Updated: 2023/08/12 22:09:12 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

bool PASS(ServParams const &server, Client *client, std::vector<std::string> const &res)
{
	if (!client)
		return (true);
	if (client->getStatus())
		return (false);
	if (!client->hasPassword() && (res.size() != 3 || res[2] != server.getPass()))
		return (true);
	else if (res[2] == server.getPass())
		client->setPassword(true);
	return (true);
}

	// throw std::invalid_argument(genErrMsg(ERR_NEEDMOREPARAMS, "*", res[1], ERR_NEEDMOREPARAMS_DESC));

bool NICK(ServParams &server, Client *client, std::vector<std::string> const &res)
{
	std::string tmp;
	if (res.size() == 3 && res[2] == "")
	{
		if (client->getStatus())
			tmp = client->getNick();
		else
			tmp = "*";
		throw std::invalid_argument(genErrMsg(ERR_NONICKNAMEGIVEN, tmp, "", ERR_NONICKNAMEGIVEN));
	}
	// else if ()
}