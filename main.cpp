/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 19:54:40 by abiru             #+#    #+#             */
/*   Updated: 2023/08/27 12:49:44 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "IrcUtils.hpp"
#include <signal.h>

// basic socket communication (server)
int main(int ac, char **av)
{
	signalHandler();
	if (ac != 3)
	{
		std::cerr << "USAGE: ./ircserv port password" << std::endl;
		return (EXIT_FAILURE);
	}
	if (!checkPort(av[1]))
	{
		std::cerr << "Bad character in port argument" << std::endl;
		return (EXIT_FAILURE);
	}
	Server Server(av[2], atoi(av[1]));
	if (!Server.start())
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
