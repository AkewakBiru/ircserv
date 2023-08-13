/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 19:54:40 by abiru             #+#    #+#             */
/*   Updated: 2023/08/13 20:11:30 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// basic socket communication (server)
int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "USAGE: ./ircserv port password" << std::endl;
		return (EXIT_FAILURE);
	}
	ServParams servParams(av[2], atoi(av[1]));
	try
	{
		servParams.checkParams();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}

	servParams.setServCreationTime();
	if (!servParams.getServAddrInfo())
		return (EXIT_FAILURE);

	if (!servParams.createSocket())
		return (EXIT_FAILURE);

	if (!servParams.listenForConn())
		return (EXIT_FAILURE);

	std::cout << "Listening ......\n";

	servParams.handleRequest();
}