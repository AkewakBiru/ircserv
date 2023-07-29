/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 21:54:20 by abiru             #+#    #+#             */
/*   Updated: 2023/07/29 21:26:59 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

ServParams::ServParams(std::string pass, int const port): _password(pass), _port(port), _servfd(-1), _res(NULL), _fdCount(0), _pfds(NULL), _clients(0), _channels(0)
{}

ServParams::~ServParams()
{
	if (_servfd >= 0)
		close(_servfd);
	if (_pfds)
		delete[] _pfds;
}

void ServParams::setPass(std::string &newPass)
{
	_password = newPass;
}

std::string const &ServParams::getPass() const
{
	return (_password);
}

int ServParams::getPort() const
{
	return (_port);
}

void ServParams::checkParams() const
{
	if (_port < 1 || _port > 65535)
		throw std::out_of_range("Port out of Range, valid range [1 - 65535]");
	if (_password.length() == 0)
		throw std::invalid_argument("Password can't be empty");
	else if (hasWhiteSpace(_password))
		throw std::invalid_argument("Password can't have whitespaces");
}

void ServParams::setServFd(int const fd)
{
	_servfd = fd;
}

int ServParams::getServFd() const
{
	return (_servfd);
}

/*
	** sets _res with the server's addrinfos
*/
bool ServParams::getServAddrInfo(void)
{
	struct addrinfo hints;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	std::stringstream sa;
	sa << _port;
	std::string strPort = sa.str();
	
	if (int status = getaddrinfo(NULL, strPort.c_str(), &hints, &_res) != 0)
	{
		std::cerr << "getaddrinfo" << gai_strerror(status);
		return (false);
	}
	return (true);
}

/*
	** creates a tcp socket
	** sets the socket to be reusable
	** assigns it a port and an IP (bind())
*/
bool ServParams::createSocket(void)
{
	struct addrinfo *p;
	int val = 1;

	for (p = _res; p != NULL; p = p->ai_next)
	{
		_servfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (_servfd == -1)
		{
			std::cerr << "socket " << strerror(errno) << std::endl;
			continue ;
		}
		if (setsockopt(_servfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) == -1)
		{
			freeaddrinfo(_res);
			std::cerr << "setsockopt " << strerror(errno) << std::endl;
			return (false);
		}
		if (bind(_servfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(_servfd);
			std::cerr << "bind " << strerror(errno) << std::endl;
			continue ;
		}
		break ;
	}
	if (p == NULL)
	{
		freeaddrinfo(_res);
		std::cerr << "Socket binding failed" << std::endl;
		return (false);
	}
	freeaddrinfo(_res);
	return (true);
}

/*
	** listens for incoming connection
*/
bool ServParams::listenForConn(void) const
{
	if (listen(_servfd, std::numeric_limits<int>::max()) == -1)
	{
		std::cerr << "Listen " << strerror(errno) << std::endl;
		return (false);
	}
	return (true);
}


bool ServParams::handleRequest(void)
{
	char msg[1024];
	Parser parser;
	struct sockaddr_storage client_addr;
	socklen_t c_len = sizeof(client_addr);
	int conn;
	void *addr;
	char ip[INET6_ADDRSTRLEN];
	
	addNewConn(_servfd);

	while (true)
	{
		int polled_fds = poll(_pfds, _fdCount, -1);
		if ( polled_fds == -1)
		{
			std::cerr << "poll: " << strerror(errno) << std::endl;
			return (EXIT_FAILURE);
		}
		for (int i=0; i<_fdCount; i++)
		{
			// is someone ready to read from the created sockets
			if (_pfds[i].revents & POLLIN)
			{
				// is that someone the server
				if (_pfds[i].fd == _servfd)
				{
					conn = accept(_servfd, (struct sockaddr *)&client_addr, &c_len);
					if (conn == -1)
					{
						std::cerr << "accept: " << strerror(errno) << std::endl;
					}
					else
					{
						if (((struct sockaddr *)&client_addr)->sa_family == AF_INET)
							addr = &((struct sockaddr_in *)&client_addr)->sin_addr;
						else
							addr = &((struct sockaddr_in6 *)&client_addr)->sin6_addr;
						std::cout << "Received connection from " << inet_ntop(client_addr.ss_family, addr, ip, INET6_ADDRSTRLEN) << " on socket: " << conn << "\r\n";
						addNewConn(conn);
					}
				}
				else
				{
					std::memset(msg, 0, 1024);
					int data = recv(_pfds[i].fd, msg, 1023, 0);
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
						removeConn(i);
					}
					else
					{
						if (!parser.isSpaces(msg))
						{
							parser.parseInput(msg, " \t");
							std::vector<std::string> const &res = parser.getRes();
							if (!isRegistered(_pfds[i].fd))
							{
								registerUser(_pfds[i].fd, res, msg);
								if (!isRegistered(_pfds[i].fd))
								{
									std::memset(msg, 0, 1024);
									std::strcpy(msg, "Incorrect password\r\n");
									send(_pfds[i].fd,msg, sizeof(msg), 0);
									memset(msg, 0, 1024);
									removeConn(i);
								}
								// register user
							}
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
			else if (_pfds[i].revents & POLLOUT)
			{
				// someone is ready to send data
			}
		}
	}
}

/*
	** whenever a new connection is made
	** array gets extended
*/
void ServParams::addNewConn(int fd)
{
	struct pollfd *tmp = new pollfd[_fdCount + 1];

	for (int i=0; i<_fdCount; i++)
	{
		tmp[i].events = _pfds[i].events;
		tmp[i].fd = _pfds[i].fd;
		tmp[i].revents = _pfds[i].revents;
	}
	if (_pfds)
		delete[] _pfds;
	_pfds = tmp;
	_pfds[_fdCount].fd = fd;
	_pfds[_fdCount].events = POLLIN;
	_fdCount++;
}

/*
	** whenever a client is disconnected
	** array gets compressed
*/
void ServParams::removeConn(int index)
{
	struct pollfd *tmp = new pollfd[_fdCount - 1];
	int j = 0;

	close(_pfds[index].fd);
	_pfds[index].fd = -1;
	for (int i=0; i<_fdCount; i++)
	{
		if (_pfds[i].fd != -1)
		{
			tmp[j].events = _pfds[i].events;
			tmp[j].fd = _pfds[i].fd;
			tmp[j].revents = _pfds[i].revents;
			j++;
		}
	}
	if (_pfds)
		delete[] _pfds;
	_pfds = tmp;
}

void ServParams::addClient(Client &client)
{
	_clients.push_back(client);
}

static bool isEqual(Client const &client, int fd)
{
	return (fd == client.getFd());
}

std::vector<Client>::iterator ServParams::findFd(std::vector<Client>& client, int fd)
{
	for (std::vector<Client>::iterator it = client.begin(); it != client.end(); ++it)
	{
		if (isEqual(*it, fd))
			return it;
	}
	return client.end();
}

void ServParams::removeClient(Client &client)
{
	std::vector<Client>::iterator finder = findFd(_clients, client.getFd());
	if (finder != _clients.end())
		_clients.erase(finder);
}

bool ServParams::isRegistered(int fd)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (isEqual(*it, fd))
			return (true);
	}
	return (false);
}

bool ServParams::registerUser(int fd, std::vector<std::string> const &res, char const *msg)
{
	std::string const &cmd = res.front();
	if (cmd != "PASS")
	{
		std::cerr << "User hasn't registered\r\n";
		return (false);
	}
	if (cmd == "NICK" || cmd == "USER")
	{
		if (cmd == "NICK")
		{
			if (res.size() == 2)
			{
				Client client;
				client.setNick(res[1]);
				client.setFd(fd);
				_clients.push_back(client);
			}
		}
		// else if (cmd == "USER")
		// {
		// 	if (res.size() >= 5)
		// 	{
				
		// 	}
		// }
	}
	return (true);
}