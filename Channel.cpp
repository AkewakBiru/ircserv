/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 21:07:13 by abiru             #+#    #+#             */
/*   Updated: 2023/08/08 21:18:38 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(): _members(0)
{}

Channel::~Channel()
{}

std::vector<Client *> *Channel::getMembers(void)
{
	return (&_members);
}
