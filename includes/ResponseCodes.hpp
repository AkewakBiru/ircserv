/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseCodes.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:36:29 by youssef           #+#    #+#             */
/*   Updated: 2023/10/23 21:07:41 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_CODES
#define RESPONSE_CODES

#define RPL_WELCOME "001"
#define RPL_YOURHOST "002"
#define RPL_CREATED "003"
#define RPL_MYINFO "004"
#define RPL_BOUNCE "005"

// MOTD
#define RPL_MOTDSTART "375"
#define RPL_MOTDSTART_DESC ":- ircserv Message of the Day -"
#define RPL_MOTD "372"
#define RPL_ENDOFMOTD "376"
#define RPL_ENDOFMOTD_DESC ":End of /MOTD command."

// PRIVMSG
#define ERR_NORECIPIENT "411"
#define ERR_NORECIPIENT_DESC ":No recipient given. PRIVMSG command requires a recipient."
#define ERR_NOTEXTTOSEND "412"
#define ERR_NOTEXTTOSEND_DESC ":No text to send. PRIVMSG command requires a message."
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_CANNOTSENDTOCHAN_DESC ":Cannot send message. Channel does not exist or is not accessible."
#define ERR_NOSUCHNICK "401"
#define ERR_NOSUCHNICK_DESC ":No such nickname. User does not exist on this server."

// WHOIS
#define RPL_WHOISUSER "311"
#define RPL_WHOISSERVER "312"
#define RPL_ENDOFWHOIS "318"
#define RPL_WHOISCHANNELS "319"

// JOIN
#define ERR_BADCHANNAME "479"
#define ERR_BADCHANNAME_DESC ":Channel is unavailable: Illegal characters"
#define ERR_LONGCHANNAME "403"
#define ERR_LONGCHANNAME_DESC ":Channel name is too long"
#define ERR_INVALIDCHANNAME "448"
#define ERR_INVALIDCHANNAME_DESC ":Cannot join channel. Channel name must start with a hash mark (#)."
#define ERR_CHANNELISFULL "471"
#define ERR_CHANNELISFULL_DESC ":Channel is full. It has reached its maximum user limit."
#define ERR_INVITEONLYCHAN "473"
#define ERR_INVITEONLYCHAN_DESC ":You must be invited to join invite-only channels."
#define ERR_BADCHANNELKEY "475"
#define ERR_BADCHANNELKEY_DESC ":Incorrect channel key. The correct password is required to join this channel."
#define RPL_TOPIC "332"
#define RPL_TOPIC_DESC ":server.hostname 332 yourNickname #channel :Current topic: Welcome to the discussion on technology!"
#define RPL_NAMREPLY "353"
#define RPL_NAMREPLY_DESC ""

// MODE
#define ERR_UNKNOWNMODE "472"
#define ERR_UNKNOWNMODE_DESC ":Unknown mode character. The specified mode is not recognized."
#define ERR_KEYSET "467"
#define ERR_KEYSET_DESC ":Channel key is set. You must provide the correct key."

// INVITE
#define ERR_USERSDISABLED "443"
#define ERR_USERSDISABLED_DESC ":The specified user is already on channel."
#define RPL_INVITING "341"
#define RPL_SUMMONING "342"

// MODE, KICK, INVITE
#define ERR_USERNOTINCHANNEL "441"
#define ERR_USERNOTINCHANNEL_DESC ":The specified user is not in the channel."
#define ERR_NOSUCHCHANNEL "403"
#define ERR_NOSUCHCHANNEL_DESC ":No such channel."
#define ERR_CHANOPRIVSNEEDED "482"
#define ERR_CHANOPRIVSNEEDED_DESC ":You need to be a channel operator to execute this command."

// TOPIC, KICK
#define ERR_NOTONCHANNEL "442"
#define ERR_NOTONCHANNEL_DESC ":You are not on that channel."

// error msgs
#define ERR_NOSUCHSERVER "402"
#define ERR_NOSUCHSERVER_DESC ":No such server"
#define ERR_NOMOTD "422"
#define ERR_NOMOTD_DESC ":MOTD File is missing"
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
#define ERR_INVALIDCAPCMD "410"
#define ERR_INVALIDCAPCMD_DESC ":Invalid CAP command"

#endif