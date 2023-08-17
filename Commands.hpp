#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <string>
#include <vector>
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
#endif