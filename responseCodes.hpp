/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseCodes.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 21:57:37 by abiru             #+#    #+#             */
/*   Updated: 2023/08/12 20:50:39 by abiru            ###   ########.fr       */
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
#endif