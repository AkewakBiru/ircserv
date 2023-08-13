/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUtils.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 21:26:35 by abiru             #+#    #+#             */
/*   Updated: 2023/08/13 19:26:33 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCUTILS_HPP
#define IRCUTILS_HPP
#include <iostream>
#include <string>
#include <cstring>
#include "responseCodes.hpp"
#include "Client.hpp"

bool hasWhiteSpace(std::string const &str);
bool hasIllegalChars(std::string const &arg);
bool userIllegalChars(std::string const &arg);
std::string toUpper(std::string const &arg, bool flag);
	// structure of the string -> ":ircserv " + err_code + optional_param + cmd + ERR_DESCRIPTION + "\r\n"
std::string genErrMsg(char const *errCode, std::string optional, std::string cmd, std::string desc);
std::string genServErrMsg(std::string nick, std::string ip, std::string desc);
#endif