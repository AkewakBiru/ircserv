/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 21:54:20 by abiru             #+#    #+#             */
/*   Updated: 2023/09/23 14:03:18 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::m_state = RUNNING;

Server::Server(std::string pass, int const port) : _creationTime(""), _password(pass),
												   _port(port), _servfd(-1), _res(NULL), _pfds(0), _clients(0), _channels(0)
{
	std::array<std::string, 10> cmds = {"NICK", "USER", "CAP", "PASS", "MOTD", "JOIN", "PRIVMSG", "PART", "KICK", "QUIT"};
	for (size_t i = 0; i < cmds.size(); i++)
	{
		_validCmds.push_back(cmds[i]);
	};
}

void Server::cleanup()
{
	// close fds
	for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
	{
		close(it->fd);
		it->fd = -1;
	}

	// delete clients
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		delete (*it);
		(*it) = NULL;
	}

	// delete channels
	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		delete (*it);
		(*it) = NULL;
	}
}

bool Server::start()
{
	signalHandler();
	try
	{
		checkParams();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	setServCreationTime();
	if (!getServAddrInfo())
		return (EXIT_FAILURE);
	if (!createSocket())
		return (EXIT_FAILURE);
	if (!listenForConn())
		return (EXIT_FAILURE);

	std::cout << "Listening ......" << std::endl;

	handleRequest();
	return (true);
}

Server::~Server()
{
	std::cout << "destructor called\n";
	cleanup();
	if (_servfd >= 0)
		close(_servfd);
	_servfd = -1;
}

void Server::setServCreationTime()
{
	std::stringstream strTime;
	time_t now = time(0);
	std::tm *d_t = std::localtime(&now);
	char dayFormat[] = "%A";
	char dayString[20];
	char monthFormat[] = "%B";
	char monthString[20];

	std::strftime(dayString, sizeof(dayString), dayFormat, d_t);
	std::strftime(monthString, sizeof(monthString), monthFormat, d_t);

	strTime << dayString << " " << monthString << " " << std::setw(2) << std::setfill('0')
			<< d_t->tm_mday << " " << d_t->tm_year + 1900 << " at " << std::setw(2) << std::setfill('0')
			<< d_t->tm_hour << ":" << std::setw(2) << std::setfill('0') << d_t->tm_min << ":" << std::setw(2)
			<< std::setfill('0') << d_t->tm_sec;
	_creationTime = strTime.str();
}

std::string &Server::getServCreationTime()
{
	return (_creationTime);
}

void Server::setPass(std::string &newPass)
{
	_password = newPass;
}

std::string const &Server::getPass() const
{
	return (_password);
}

int Server::getPort() const
{
	return (_port);
}

void Server::checkParams() const
{
	if (_port < 1 || _port > 65535)
		throw std::out_of_range("Port out of Range, valid range [1 - 65535]");
	if (_password.length() == 0)
		throw std::invalid_argument("Password can't be empty");
	else if (hasWhiteSpace(_password))
		throw std::invalid_argument("Password can't have whitespaces");
}

void Server::setServFd(int const fd)
{
	_servfd = fd;
}

int Server::getServFd() const
{
	return (_servfd);
}

/*
 ** sets _res with the server's addrinfos
 */
bool Server::getServAddrInfo(void)
{
	if (Server::m_state == STOPPED)
		return (false);
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
 ** sets it to be non-blocking
 */
bool Server::createSocket(void)
{
	struct addrinfo *p;
	int val = 1;

	for (p = _res; p != NULL; p = p->ai_next)
	{
		if (Server::m_state == STOPPED)
		{
			freeaddrinfo(_res);
			return (false);
		}
		_servfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (_servfd == -1)
		{
			std::cerr << "socket: " << strerror(errno) << std::endl;
			continue;
		}
		if (setsockopt(_servfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) == -1)
		{
			freeaddrinfo(_res);
			std::cerr << "setsockopt: " << strerror(errno) << std::endl;
			return (false);
		}
		if (bind(_servfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(_servfd);
			std::cerr << "bind: " << strerror(errno) << std::endl;
			continue;
		}
		if (fcntl(_servfd, F_SETFL, O_NONBLOCK) == -1)
		{
			close(_servfd);
			std::cerr << "fcntl: " << strerror(errno) << std::endl;
			continue;
		}
		break;
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
bool Server::listenForConn(void) const
{
	if (Server::m_state == STOPPED)
		return (false);
	if (listen(_servfd, std::numeric_limits<int>::max()) == -1)
	{
		std::cerr << "Listen " << strerror(errno) << std::endl;
		return (false);
	}
	return (true);
}

bool Server::deleteConnection(int fd)
{
	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		std::vector<Client *> *tmp = (*it)->getMembers();
		for (std::vector<Client *>::iterator b = tmp->begin(); b != tmp->end(); b++)
		{
			if ((*b)->getFd() == fd)
			{
				(*b)->setFd(-1);
				tmp->erase(b);
				break;
			}
		}
	}

	// remove from clients vector
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if ((*it)->getFd() == fd)
		{
			(*it)->setFd(-1);
			delete (*it);
			_clients.erase(it);
			break;
		}
	}

	// remove from _pfds
	for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
	{
		if (it->fd == fd)
		{
			it->fd = -1;
			_pfds.erase(it);
			break;
		}
	}
	return (true);
}

/*
 **
 */
bool Server::sendMsgAndCloseConnection(std::string const &msg, Client *client)
{
	std::cout << "Lost connection to " << client->getIpAddr() << " on socket " << client->getFd() << std::endl;
	sendMsg(client->getFd(), msg);
	close(client->getFd());
	deleteConnection(client->getFd());
	return (true);
}

void Server::acceptNewConnection()
{
	struct sockaddr_storage clientAddr;
	socklen_t cLen = sizeof(clientAddr);
	int conn = accept(_servfd, (struct sockaddr *)&clientAddr, &cLen);

	if (conn == -1)
		std::cerr << "accept: " << strerror(errno) << std::endl;
	else
		addNewClient(conn, &clientAddr);
}

void Server::addNewClient(int fd, struct sockaddr_storage *client_addr)
{

	struct pollfd newConnection = {fd, POLLIN | POLLOUT, 0};
	_pfds.push_back(newConnection);

	Client *client = new Client();
	client->setIpAddr(client_addr);
	std::cout << "Received connection from " << client->getIpAddr() << " on socket: " << fd << "\r\n";
	client->setJoinedTime(std::time(0));
	client->setFd(fd);
	_clients.push_back(client);
}

bool Server::handleRequest(void)
{
	char msg[512];
	int polled_fds;
	ssize_t data;
	size_t i;

	// add the server to the pfds vector
	struct pollfd serv = {_servfd, POLLIN, 0};
	_pfds.push_back(serv);

	while (Server::m_state == RUNNING)
	{
		polled_fds = poll(&_pfds[0], _pfds.size(), -1);
		if (polled_fds == -1)
		{
			std::cerr << "poll: " << strerror(errno) << std::endl;
			return (EXIT_FAILURE);
		}
		if (polled_fds == 0)
			continue;
		for (i = 0; i < _pfds.size(); i++)
		{
			// is someone ready to read from the created sockets
			if (_pfds[i].revents & POLLIN)
			{
				// is that someone the server
				if (_pfds[i].fd == _servfd)
				{
					acceptNewConnection();
					continue;
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
							sendMsg(_pfds[i].fd, "bye\r\n");
						else
							std::cerr << "recv: " << strerror(errno) << std::endl;
						// remove the fd not responding from the _pfds, clients and channel vector
						std::cout << "Lost connection to " << _clients[i - 1]->getIpAddr() << " on socket " << _pfds[i].fd << std::endl;
						close(_pfds[i].fd);
						deleteConnection(_pfds[i].fd);
					}
					else
					{
						msg[510] = '\r';
						msg[511] = '\n';

						if (!preParseInput(_clients[i - 1], msg, data))
							continue;
						// adds msg to the dataBuffer
						_clients[i - 1]->addToBuffer(msg);
						processBuffer(_clients[i - 1]);
						std::memset(msg, 0, sizeof(msg));
					}
				}
			}
			else if (_pfds[i].revents & POLLOUT && _pfds[i].fd != _servfd)
			{
				if (_clients[i - 1]->getOutgoingMsgBuffer().length() > 0)
				{
					if (_clients[i - 1]->getMsgDest()[0] == '#')
						sendToChannel(_clients[i - 1]);
					else
						sendMsg(getNicksFd(_clients[i - 1]->getMsgDest()), _clients[i - 1]->getOutgoingMsgBuffer());
				}
				_clients[i - 1]->setOutgoingMsgBuffer("");
			}
		}
		removeNonRespClients();
		removeEmptyChannels();
	}
	return (true);
}

void Server::sendToChannel(Client *sender)
{
	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if ((*it)->getName() == sender->getMsgDest())
		{
			std::vector<Client *> *members = (*it)->getMembers();
			for (std::vector<Client *>::iterator it2 = members->begin(); it2 != members->end(); members++)
				sendMsg((*it2)->getFd(), sender->getOutgoingMsgBuffer());
		}
	}
}

void Server::executeCmd(Client *client, std::vector<std::string> const &res)
{
	bool (*funcs[])(Server &, Client *, std::vector<std::string> const &) = {&NICK, &USER, &CAP, &PASS, &MOTD, &JOIN, &PRIVMSG};

	// cmd validity is checked for authenticated client only
	if (!isValidCmd(toUpper(res[1], false), _validCmds))
		throw std::invalid_argument(genErrMsg(ERR_UNKNOWNCOMMAND, client->getNick(), res[1], ERR_UNKNOWNCOMMAND_DESC));

	for (size_t i = 0; i < 7; i++)
	{
		if (_validCmds[i] == toUpper(res[1], false))
		{
			try
			{
				(*funcs[i])(*this, client, res);
			}
			catch (const std::exception &e)
			{
				(void)e;
				throw;
			}
			break;
		}
	}
	return;
}

/*
 ** loops through the execBuffer queue and executes commands and dequeues it
 */
void Server::processBuffer(Client *client)
{
	Parser parser;

	if (client->getDataBuffer().empty())
		return;
	while (client->getDataBuffer().size() && client->getState() == UP)
	{
		try
		{
			parser.parseInput(client->getDataBuffer().front());
		}
		catch (std::exception const &e)
		{
			client->rmvfromBuf();
			parser.resetRes();
			sendMsg(client->getFd(), e.what());
			continue;
		}
		client->rmvfromBuf();
		std::vector<std::string> const &res = parser.getRes();

		if (!isRegistered(client->getFd()))
		{
			try
			{
				registerUser(client, res);
			}
			catch (const std::exception &e)
			{
				sendMsg(client->getFd(), e.what());
			}
		}
		else
		{
			try
			{
				executeCmd(client, res);
			}
			catch (const std::exception &e)
			{
				sendMsg(client->getFd(), e.what());
			}
		}
		parser.resetRes();
	}
	if (client->getState() == DOWN)
		sendMsgAndCloseConnection("", client);
}

void Server::addClient(Client *client)
{
	_clients.push_back(client);
}

static bool isEqual(Client const *client, int fd)
{
	return (fd == client->getFd());
}

std::vector<Client *>::iterator Server::findFd(std::vector<Client *> &client, int fd)
{
	for (std::vector<Client *>::iterator it = client.begin(); it != client.end(); ++it)
	{
		if (isEqual(*it, fd))
			return it;
	}
	return client.end();
}

/*
 ** returns fd associated with a nick
 */
ssize_t Server::getNicksFd(std::string nick)
{
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if ((*it)->getNick() == nick)
			return ((*it)->getFd());
	}
	return (-1);
}

void Server::removeClient(Client *client)
{
	std::vector<Client *>::iterator finder = findFd(_clients, client->getFd());
	if (finder != _clients.end())
	{
		_clients.erase(finder);
		delete client;
	}
}

bool Server::isRegistered(int fd)
{
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (isEqual(*it, fd))
		{
			if ((*it)->getStatus())
				return (true);
			break;
		}
	}
	return (false);
}

bool Server::registerUser(Client *client, std::vector<std::string> const &res)
{
	std::string cmds[] = {"NICK", "USER", "CAP", "PASS"};
	bool (*funcs[])(Server &, Client *, std::vector<std::string> const &) = {&NICK, &USER, &CAP, &PASS};
	std::string const &cmd = toUpper(res[1], false);

	for (size_t i = 0; i < 4; i++)
	{
		if (cmds[i] == cmd)
		{
			try
			{
				(*funcs[i])(*this, client, res);
			}
			catch (const std::exception &e)
			{
				(void)e;
				throw;
			}
			// if client has a correct password, nick and user, set their status as joined
			if (client->hasPassword() && client->getUserName().length() > 0 && client->getNick().length() > 0)
			{
				client->setStatus(true);
				sendWelcomingMsg(client);
			}
			return (true);
		}
	}
	throw std::invalid_argument(genErrMsg(ERR_NOTREGISTERED, "*", "", ERR_NOTREGISTERED_DESC));
}

void Server::sendWelcomingMsg(Client *client)
{
	std::string msg = "";

	msg.append(":ircserv ").append(RPL_WELCOME).append(" :Welcome to the Internet Relay Network ").append(client->getNick()).append("!").append(client->getUserName()).append("@").append(client->getIpAddr()).append("\n");
	msg.append(":ircserv ").append(RPL_YOURHOST).append(" :Your host is ircserv, running version 10.0\n");
	msg.append(":ircserv ").append(RPL_CREATED).append(" :This server was created ").append(_creationTime).append("\n");
	msg.append(":ircserv ").append(RPL_MYINFO).append(" :ircserv 10.0\r\n");
	sendMsg(client->getFd(), msg);

	Parser parser;
	parser.parseInput("MOTD");
	MOTD(*this, client, parser.getRes());
}

void Server::setStatus(bool status)
{
	_status = status;
}

int Server::getStatus() const
{
	return (_status);
}

/*
 ** non-registered and unresponding clients (timeout = 1 minute)
 ** will be removed from the server
 */
void Server::removeNonRespClients()
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (!isRegistered(_clients[i]->getFd()) && std::time(0) - _clients[i]->getJoinedTime() >= 60)
		{
			sendMsgAndCloseConnection(genServErrMsg("", _clients[i]->getIpAddr(), "Registration Timeout"), _clients[i]);
			i--;
		}
	}
}

void Server::removeEmptyChannels()
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (!_channels[i]->getMembers()->size())
		{
			delete _channels[i];
			i--;
		}
	}
}
