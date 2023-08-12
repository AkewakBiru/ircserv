#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <string>
#include <vector>
#include "Client.hpp"
#include "Server.hpp"
#include "Parser.hpp"

class ServParams;

bool NICK(ServParams &server, Client *client, std::vector<std::string> const &res);
bool PASS(ServParams const &server, Client *client, std::vector<std::string> const &res);
#endif