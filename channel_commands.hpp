
#ifndef CHANNEL_COMMANDS_HPP
#define CHANNEL_COMMANDS_HPP
#include "Channel.hpp"
#include "Client.hpp"
#include <vector>
#include <string>


class Client;

class ChannelCommands
{
public:
    ChannelCommands();
    ~ChannelCommands();

    void invite(Channel& channel, Client& inviter, Client& invitee);
    void kick(Channel& channel, Client& kicker, Client& kickee);
    void setTopic(Channel& channel, Client& client, const std::vector<std::string>& messages);
    void manageMods(Channel& channel, Client& client, const std::vector<std::string>& messages);
    void executeCommand(std::vector<std::string> messages, User *user);

};

#endif