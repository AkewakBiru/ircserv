/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 20:34:42 by abiru             #+#    #+#             */
/*   Updated: 2023/07/27 21:09:34 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Client.hpp"
#include <vector>

class Channel
{
	public:
		Channel();
		~Channel();

	private:
		std::vector<Client *> _members;
		Client *_operator;
};
#endif