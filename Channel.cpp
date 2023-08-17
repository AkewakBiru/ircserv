/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 21:07:13 by abiru             #+#    #+#             */
/*   Updated: 2023/08/17 21:33:47 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(): _members(0), _operator(0)
{}

Channel::~Channel()
{}

std::vector<Client *> *Channel::getMembers(void)
{
	(void) _operator;
	return (&_members);
}
