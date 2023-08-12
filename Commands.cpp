/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 11:55:48 by abiru             #+#    #+#             */
/*   Updated: 2023/08/12 12:09:01 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

bool PASS(ServParams const &server, Client *client, std::vector<std::string> const &res)
{
	if (!client)
		return (false);
	if (!client->hasPassword() && (res.size() != 3 || res[2] != server.getPass()))
		return (false);
	else if (res[2] == server.getPass())
		client->setPassword(true);
	return (true);
}
