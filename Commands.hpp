#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <string>
#include <vector>
#include "responseCodes.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Parser.hpp"

class Server;

void NICK(Server &server, Client *client, std::vector<std::string> const &res);
bool PASS(Server const &server, Client *client, std::vector<std::string> const &res);
void USER(Server const &server, Client *client, std::vector<std::string> const &res);
#endif