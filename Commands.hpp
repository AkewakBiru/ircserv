#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "responseCodes.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Parser.hpp"

class Server;

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


void INVITE(Server &server, Client *client, std::vector<std::string> const &res);
void KICK(Server &server, Client *client, std::vector<std::string> const &res);
void setTopic(Server &server, Client *client, std::vector<std::string> const &res);
void manageMods(Server &server, Client *client, std::vector<std::string> const &res);
void executeCommand(Server &server, Client *client, std::vector<std::string> const &res);
#endif