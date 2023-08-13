/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseCodes.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 21:57:37 by abiru             #+#    #+#             */
/*   Updated: 2023/08/13 13:12:19 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_CODES
#define RESPONSE_CODES

#define RPL_WELCOME "001"
#define RPL_YOURHOST "002"
#define RPL_CREATED "003"
#define RPL_MYINFO "004"
#define RPL_BOUNCE "005"

// error msgs
#define ERR_NEEDMOREPARAMS "461"
#define ERR_NEEDMOREPARAMS_DESC ":Not enough parameters"
#define ERR_ALREADYREGISTRED "462"
#define ERR_ALREADYREGISTRED_DESC ":You may not reregister"
#define ERR_NONICKNAMEGIVEN "431"
#define ERR_NONICKNAMEGIVEN_DESC ":No nickname given"
#define ERR_ERRONEUSNICKNAME "432"
#define ERR_ERRONEUSNICKNAME_DESC ":Nickname is unavailable: Illegal characters"
#define ERR_TOOLONGNICKNAME ":Nickname is too long"
#define ERR_NICKNAMEINUSE "433"
#define ERR_NICKNAMEINUSE_DESC ":Nickname is already in use"
#define ERR_NOTREGISTERED "451"
#define ERR_NOTREGISTERED_DESC ":You have not registered"
#define ERR_UNKNOWNCOMMAND "421"
#define ERR_UNKNOWNCOMMAND_DESC ":Unknown command"
#define ERR_UNKNOWNCOMMAND_ILLEGAL ":Illegal characters in message"
#define ERR_UNAUTHORIZED_ACCESS "You are not authorized to connect to this server"
#define ERR_INCORRECT_PASSWORD "Incorrect password! "
#define ERR_CMD_NOT_PASSWORD "Please send your password as the first message."
#endif