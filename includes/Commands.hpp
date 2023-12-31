#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "ResponseCodes.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Parser.hpp"
#include "Channel.hpp"
#include "IrcUtils.hpp"

class Server;

class Channel;

class Client;

bool NICK(Server &server, Client *client, std::vector<std::string> const &res);
bool PASS(Server &server, Client *client, std::vector<std::string> const &res);
bool USER(Server &server, Client *client, std::vector<std::string> const &res);
bool CAP(Server &server, Client *client, std::vector<std::string> const &res);
bool MOTD(Server &server, Client *client, std::vector<std::string> const &res);
bool PRIVMSG(Server &server, Client *client, std::vector<std::string> const &res);
bool JOIN(Server &server, Client *client, std::vector<std::string> const &res);
bool QUIT(Server &server, Client *client, std::vector<std::string> const &res);
bool NAMES(Server &server, Client *client, Channel *channel);
bool WHOIS(Server &server, Client *client, std::vector<std::string> const &res);
bool MODE(Server &server, Client *client, std::vector<std::string> const &res);
bool INVITE(Server &server, Client *client, std::vector<std::string> const &res);
bool KICK(Server &server, Client *client, std::vector<std::string> const &res);
bool PART(Server &server, Client *client, std::vector<std::string> const &res);
bool TOPIC(Server &server, Client *client, std::vector<std::string> const &res);
bool PING(Server &server, Client *client, std::vector<std::string> const &res);
#endif