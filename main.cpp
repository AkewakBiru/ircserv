/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 19:54:40 by abiru             #+#    #+#             */
/*   Updated: 2023/08/15 21:46:05 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "IrcUtils.hpp"

// basic socket communication (server)
int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "USAGE: ./ircserv port password" << std::endl;
		return (EXIT_FAILURE);
	}

	Server Server(av[2], atoi(av[1]));
	getServerInstance(&Server);
	struct sigaction sa = { sigHandle, 0, 0};
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, 0);
	sigaction(SIGQUIT, &sa, 0);
	try
	{
		Server.checkParams();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}

	Server.setServCreationTime();
	if (!Server.getServAddrInfo())
		return (EXIT_FAILURE);

	if (!Server.createSocket())
		return (EXIT_FAILURE);

	if (!Server.listenForConn())
		return (EXIT_FAILURE);

	std::cout << "Listening ......\n";

	Server.handleRequest();
}