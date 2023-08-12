/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 21:54:20 by abiru             #+#    #+#             */
/*   Updated: 2023/08/13 00:51:32 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

ServParams::ServParams(std::string pass, int const port): _password(pass), _port(port), _servfd(-1), _res(NULL), _fdCount(0), _pfds(0), _clients(0), _channels(0)
{}

ServParams::~ServParams()
{
	if (_servfd >= 0)
		close(_servfd);
	// if (_pfds)
	// 	delete[] _pfds;
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

bool ServParams::deleteConnection(int fd)
{
	for (std::vector<Channel *>::iterator it=_channels.begin(); it != _channels.end(); it++)
	{
		std::vector<Client *> *tmp = (*it)->getMembers();
		for (std::vector<Client *>::iterator b=tmp->begin(); b != tmp->end(); b++)
		{
			if ((*b)->getFd() == fd)
			{
				(*b)->setFd(-1);
				delete(*b);
				tmp->erase(b);
				break ;
			}
		}
	}

	// remove from clients vector
	for (std::vector<Client *>::iterator it=_clients.begin(); it != _clients.end(); it++)
	{
		if ((*it)->getFd() == fd)
		{
			(*it)->setFd(-1);
			delete(*it);
			_clients.erase(it);
			break ;
		}
	}
	
	// remove from _pfds
	for (std::vector<pollfd>::iterator it=_pfds.begin(); it!=_pfds.end(); it++)
	{
		if (it->fd == fd)
		{
			it->fd = -1;
			_pfds.erase(it);
			break ;
		}
	}
	return (true);
}

/*
	** 
*/
bool ServParams::sendMsgAndCloseConnection(std::string const &msg, size_t index)
{
	std::cout << "Lost connection to " << _clients[index-1]->getIpAddr() << " on socket " << _pfds[index].fd << std::endl;
	send(_pfds[index].fd, msg.c_str(), msg.length(), 0);
	close(_pfds[index].fd);
	deleteConnection(_pfds[index].fd);
	return (true);
}

bool ServParams::handleRequest(void)
{
	fcntl(_servfd, F_SETFL, O_NONBLOCK);
	char msg[512];
	Parser parser;
	struct sockaddr_storage client_addr;
	socklen_t c_len = sizeof(client_addr);
	int conn;
	int polled_fds;
	int data;
	size_t i;

	// add the server to the pfds vector
	struct pollfd serv;
	serv.fd = _servfd;
	serv.events = POLLIN;
	serv.revents = 0;
	_pfds.push_back(serv);

	while (true)
	{
		polled_fds = poll(&_pfds[0], _pfds.size(), -1);
		if ( polled_fds == -1)
		{
			std::cerr << "poll: " << strerror(errno) << std::endl;
			return (EXIT_FAILURE);
		}
		for (i=0; i<_pfds.size(); i++)
		{
			// is someone ready to read from the created sockets
			if (_pfds[i].revents & POLLIN)
			{
				// is that someone the server
				if (_pfds[i].fd == _servfd)
				{
					conn = accept(_servfd, (struct sockaddr *)&client_addr, &c_len);
					if (conn == -1)
						std::cerr << "accept: " << strerror(errno) << std::endl;
					else
					{
						struct pollfd newConnection;
						newConnection.fd = conn;
						newConnection.events = POLLIN | POLLOUT;
						newConnection.revents = 0;
						_pfds.push_back(newConnection);
						Client *client = new Client();
						client->setIpAddr(&client_addr);
						std::cout << "Received connection from " << client->getIpAddr() << " on socket: " << conn << "\r\n";
						client->setJoinedTime(std::time(0));
						client->setFd(conn);
						_clients.push_back(client);
						// capability request from irssi should have the below response
						send(conn, (void *)"CAP * ACK multi-prefix\r\n", 24, 0);
					}
				}
				else
				{
					std::memset(msg, 0, 512);
					data = recv(_pfds[i].fd, msg, 510, 0);
					// here server reads data from the sockets and parses it, 
					// if syntax is right, performs op on it
					// else sends proper error message
					if (data <= 0)
					{
						if (data == 0)
							send(_pfds[i].fd, (void *)"bye\r\n", 5, 0);
						else
							std::cerr << "recv: " << strerror(errno) << std::endl;
						// remove the fd not responding from the _pfds, clients and channel vector
						std::cout << "Lost connection to " << _clients[i-1]->getIpAddr() << " on socket " << _pfds[i].fd << std::endl;
						close(_pfds[i].fd);
						deleteConnection(_pfds[i].fd);
					}
					else
					{
						// Client sends message to the server
						// Client should have an outGoingBuffer to save data in (when intending to send a message)
						// Client has 60 sec window to register
						msg[510] = '\r';
						msg[511] = '\n';
						std::cout << msg;
						if (!parser.isSpaces(msg))
						{
							parser.parseInput(msg);
							std::vector<std::string> const &res = parser.getRes();

							if (!isRegistered(_pfds[i].fd))
							{
								if (std::time(0) - _clients[i-1]->getJoinedTime() >= 60)
									sendMsgAndCloseConnection(genServErrMsg("", _clients[i-1]->getIpAddr(), "Registration Timeout"), i);
								try
								{
									registerUser(_pfds[i].fd, res, msg);
								}
								catch(const std::exception& e)
								{
									std::string tmp = e.what();
									send(_pfds[i].fd, tmp.c_str(), tmp.length(), 0);
								}
								parser.resetRes();
							}
						}
						parser.resetRes();
						memset(msg, 0, sizeof(msg));
					}
				}
			}
			// else if (i->revents & POLLOUT)
			// {
			// 	// someone is ready to send data
			// }
		}
	}
}

void ServParams::addClient(Client *client)
{
	_clients.push_back(client);
}

static bool isEqual(Client const *client, int fd)
{
	return (fd == client->getFd());
}

std::vector<Client *>::iterator ServParams::findFd(std::vector<Client *>& client, int fd)
{
	for (std::vector<Client *>::iterator it = client.begin(); it != client.end(); ++it)
	{
		if (isEqual(*it, fd))
			return it;
	}
	return client.end();
}

/*
	
*/
ssize_t ServParams::getNicksFd(std::string nick)
{
	for (std::vector<Client *>::iterator it = _clients.begin(); it!=_clients.end(); it++)
	{
		if ((*it)->getNick() == nick)
			return ((*it)->getFd());
	}
	return (-1);
}

void ServParams::removeClient(Client *client)
{
	std::vector<Client *>::iterator finder = findFd(_clients, client->getFd());
	if (finder != _clients.end())
	{
		_clients.erase(finder);
		delete client;
	}
}

bool ServParams::isRegistered(int fd)
{
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (isEqual(*it, fd))
		{
			if ((*it)->getStatus())
				return (true);
			break ;
		}
	}
	return (false);
}

bool ServParams::registerUser(int fd, std::vector<std::string> const &res, char const *msg)
{
	std::string const &cmd = toUpper(res[1], false);

	Client *client = *(findFd(_clients, fd));

	// if client has a correct password, nick and user, set their status as joined
	if (client->hasPassword() && client->getUserName().length() > 0 && client->getNick().length() > 0)
		client->setStatus(true);
	if (cmd == "NICK" || cmd == "USER" || cmd == "PASS")
	{
		if (cmd == "PASS")
		{
			try
			{
				PASS(*this, client, res);
			}
			catch (std::exception const &e)
			{
				(void)e;
				throw ;
			}
		}
		else if (cmd == "NICK")
		{
			try
			{
				NICK(*this, client, res);
			}
			catch (std::exception const &e)
			{
				(void)e;
				throw ;
			}
			// if (res.size() == 3 && res[2] == "")
			// 	throw std::invalid_argument(genErrMsg(ERR_NEEDMOREPARAMS, "*", res[1], ERR_NEEDMOREPARAMS_DESC));
		}
	}
	else
		throw std::invalid_argument(genErrMsg(ERR_NOTREGISTERED, "*", "", ERR_NOTREGISTERED_DESC));
	if (client->getUserName().length() > 0 && client->getNick().length() > 0 && !client->hasPassword())
	{
		throw std::runtime_error(genServErrMsg(client->getNick(), client->getIpAddr(), "You are not authorized to connect to this server"));
	}
	return (true);
}
