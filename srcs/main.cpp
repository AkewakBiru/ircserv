/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 19:54:40 by abiru             #+#    #+#             */
/*   Updated: 2023/11/07 22:59:11 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/IrcUtils.hpp"
#include <signal.h>

int main(int ac, char **av)
{
	signalHandler();
	if (ac != 3)
	{
		printError("USAGE: ./ircserv port password", "");
		return (EXIT_FAILURE);
	}
	if (!checkPort(av[1]))
	{
		printError("Bad character in port argument", ": Valid Range [ 1-65535 ]");
		return (EXIT_FAILURE);
	}
	Server Server(av[2], atoi(av[1]));
	if (!Server.start())
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
