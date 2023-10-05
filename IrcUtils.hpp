/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUtils.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youssef <youssef@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 21:26:35 by abiru             #+#    #+#             */
/*   Updated: 2023/10/05 15:04:41 by youssef          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCUTILS_HPP
#define IRCUTILS_HPP
#include <iostream>
#include <string>
#include <cstring>
#include <signal.h>
#include "responseCodes.hpp"
#include "Client.hpp"
#include "Server.hpp"

class Server;

bool hasWhiteSpace(std::string const &str);
bool hasIllegalChars(std::string const &arg);
bool userIllegalChars(std::string const &arg);
std::string toUpper(std::string const &arg, bool flag);
	// structure of the string -> ":ircserv " + err_code + optional_param + cmd + ERR_DESCRIPTION + "\r\n"
std::string genErrMsg(char const *errCode, std::string optional, std::string cmd, std::string desc);
std::string genServErrMsg(std::string nick, std::string ip, std::string desc);
void sigHandle(int sig);
void signalHandler(void);
void sendMsg(int fd, std::string msg);
Server *getServerInstance(Server *instance);
bool isSpaces(std::string const &str);
std::string &rtrim(std::string &str, std::string const &set);
bool isValidCmd(std::string const &cmd, std::vector<std::string> const &cmdList);
bool preParseInput(Client *client, char const *msg, ssize_t size);
bool checkPort(std::string arg);
void sendToRecipients(std::string formatedMessage, Client *client, Channel *channel, int fd);
#endif