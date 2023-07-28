/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 19:54:40 by abiru             #+#    #+#             */
/*   Updated: 2023/07/28 14:23:08 by abiru            ###   ########.fr       */
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

	if (!servParams.getServAddrInfo())
		return (EXIT_FAILURE);

	if (!servParams.createSocket())
		return (EXIT_FAILURE);

	if (!servParams.listenForConn())
		return (EXIT_FAILURE);

	std::cout << "Listening ......\n";

	struct pollfd pfds[120];
	int fd_count = 1;
	pfds[0].fd = servParams.getServFd();
	pfds[0].events = POLLIN;
	char msg[1024];
	Parser parser;

	struct sockaddr_storage client_addr;
	socklen_t c_len = sizeof(client_addr);
	int conn;

	while (true)
	{
		int polled_fds = poll(pfds, fd_count, -1);
		if ( polled_fds == -1)
		{
			std::cerr << "poll: " << strerror(errno) << std::endl;
			return (EXIT_FAILURE);
		}
		for (int i=0; i<fd_count; i++)
		{
			// is someone ready to read from the created sockets
			if (pfds[i].revents & POLLIN)
			{
				// is that someone the server
				if (pfds[i].fd == servParams.getServFd())
				{
					conn = accept(servParams.getServFd(), (struct sockaddr *)&client_addr, &c_len);
					if (conn == -1)
					{
						std::cerr << "accept: " << strerror(errno) << std::endl;
					}
					else
					{
						void *addr;
						char ip[INET6_ADDRSTRLEN];
						if (((struct sockaddr *)&client_addr)->sa_family == AF_INET)
							addr = &((struct sockaddr_in *)&client_addr)->sin_addr;
						else
							addr = &((struct sockaddr_in6 *)&client_addr)->sin6_addr;
						std::cout << "Received connection from " << inet_ntop(client_addr.ss_family, addr, ip, INET6_ADDRSTRLEN) << " on socket: " << conn << "\r\n";
						pfds[fd_count].fd = conn;
						pfds[fd_count].events = POLLIN;
						fd_count++;
					}
				}
				else
				{
					int data = recv(pfds[i].fd, msg, 1023, 0);
					// here server reads data from the sockets and parses it, 
					// if syntax is right, performs op on it
					// else sends proper error message
					if (data <= 0)
					{
						if (data == 0)
						{
							std::cout << "bye\r\n";
						}
						else
							std::cerr << "recv: " << strerror(errno) << std::endl;
						
						close(pfds[i].fd);
						fd_count--;
					}
					else
					{
						parser.parseInput(msg, " \t");
						std::vector<std::string> const &res = parser.getRes();
						if (!servParams.isRegistered(pfds[i].fd))
						{
							servParams.registerUser(pfds[i].fd, res);
							// register user
						}
						// if (res.front() == "NICK")
						// {
						// 	if (res.size() == 2)
						// 		std::cout << "user wants to register\r\n";
						// }
						// for (std::vector<std::string>::const_iterator i=res.begin(); i!= res.end(); ++i)
						// {
						// 	std::cout << "[" << *i << "]\t";
						// }
						// std::cout << '\n';
						parser.resetRes();
						memset(msg, 0, sizeof(msg));
						// strcpy(msg, "received\r\n");
						// send(pfds[i].fd, msg, sizeof(msg), 0);
						// memset(msg, 0, sizeof(msg));
					}
				}
			}
			else if (pfds[i].revents & POLLOUT)
			{
				// someone is ready to send data
			}
		}
	}
}
