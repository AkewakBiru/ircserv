/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 20:34:42 by abiru             #+#    #+#             */
/*   Updated: 2023/08/16 14:38:59 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Client.hpp"
#include <vector>

class Client;

class Channel
{
	public:
		Channel();
		~Channel();
		
		std::vector<Client *> *getMembers(void);

	private:
		std::vector<Client *> _members;
		Client *_operator;
};
#endif