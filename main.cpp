/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 19:54:40 by abiru             #+#    #+#             */
/*   Updated: 2023/08/20 18:29:28 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "IrcUtils.hpp"
#include <signal.h>

// basic socket communication (server)
int main(int ac, char **av)
{
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
	getServerInstance(&Server);
	struct sigaction sa;
	sa.sa_flags = 0;
	sa.sa_handler = sigHandle;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, 0);
	sigaction(SIGQUIT, &sa, 0);
	if (!Server.start())
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
