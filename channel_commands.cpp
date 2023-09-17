#include "channel_commands.hpp"
#include "Server.hpp"  

ChannelCommands::ChannelCommands() {}

ChannelCommands::~ChannelCommands() {}

void ChannelCommands::invite(Channel& channel, Client& inviter, Client& invitee)
{

    if (channel.getMode('i') && !inviter.is_op(channel))
        return;

    std::string nick = invitee.getNickName();

    for (auto& user : Server::serverInstance->getUsers())
    {
        if (user->getNickName() == nick)
        {
            user->setInvited(channel, true);
            Server::serverInstance->sendMsg(user->getUserFd(), ":" + inviter.getNickName() + " INVITE " + nick + " " + channel.getName() + "\r\n");
            break;
        }
    }
}


void ChannelCommands::kick(Channel& channel, Client& kicker, Client& kickee)
{
    std::string nick = kickee.getNickName();

    if (!kicker.is_op(channel))
        return;

    std::vector<Client *> users = channel.getMembers();

    for (auto& user : users)
    {
        if (user->getNickName() == nick)
        {
            // Send a message to the channel notifying that the user has been kicked.
            channel.sendMessage(kicker.getNickName() + " has kicked " + nick + " from " + channel.getName() + "!", "");

            // Send a message to the kicked user notifying that they have been removed from the channel.
            Server::serverInstance->sendMsg(user->getUserFd(), ":" + user->getNickName() + " PART :" + channel.getName() + "\n");

            channel.removeUser(user); 
            break;
        }
    }
}


void ChannelCommands::setTopic(Channel& channel, Client& client, const std::vector<std::string>& messages)
{
    // Logic for the topic command based on provided logic
    if (messages.size() < 3 || channel.getName() != messages[1])
        return;

    if (messages.size() == 2)
    {
        channel.sendMessage("IRC: 331 " + client.getNickName() + " " + channel.getName() + " " + channel.getTopic(), "");
        return;
    }

    if (channel.getMode('t') && !client.is_op(channel))
    {
        Server::serverInstance->sendMsg(client.getUserFd(), ":ASY 482 " + client.getNickName() + " " + channel.getName() + " :You're not channel operator\r\n");
        return;
    }

    unsigned int startIndex = (messages[2] == ":") ? 3 : 2;
    std::string topic = "";

    if (messages.size() > 3 || messages[2] != ":")
    {
        topic = messages[startIndex];
        for (unsigned int i = startIndex + 1; i < messages.size(); i++)
            topic += " " + messages[i];
    }

    channel.setTopic(topic);
    channel.sendMessage("IRC: 332 " + client.getNickName() + " " + channel.getName() + " " + channel.getTopic(), "");
}


void ChannelCommands::manageMods(Channel& channel, Client& client, const std::vector<std::string>& messages)
{
    // Logic for the mode command based on provided logic
    bool mode_bool = false;

    if (messages.size() < 2 || channel.getName() != messages[1] || !client.is_op(channel))
        return;

    if (messages.size() == 2)
    {
        std::string mode_str = "";
        if (channel.getMode('o'))
            mode_str += "o";
        if (channel.getMode('i'))
            mode_str += "i";
        if (channel.getMode('t'))
            mode_str += "t";
        if (channel.getMode('k'))
            mode_str += "k";
        if (channel.getMode('l'))
        {
            mode_str += "l ";
            mode_str += std::to_string(channel.getMaxUsers());
        }

        Server::serverInstance->sendMsg(client.getUserFd(), "324 " + client.getNickName() + " " + channel.getName() + " +" + mode_str + "\r\n");
    }

    std::string mode = messages[2];
    std::string mode_str = "";

    if (mode.length() < 2 || (mode[0] != '+' && mode[0] != '-'))
        return;

    for (unsigned int i = 0; i < mode.length(); i++)
    {
        if (mode[i] == '+')
        {
            mode_bool = true;
            mode_str += "+";
        }
        else if (mode[i] == '-')
        {    
            mode_bool = false;
            mode_str += "-";
        }
        else if (mode[i] == 'o' && messages.size() > 3)
        {
            std::string nick = messages[3];
            for (auto& user : Server::serverInstance->getUsers())
            {
                if (user->getNickName() == nick)
                {
                    user->setChannelOp(channel, mode_bool);
                    if (user->is_op(channel))
                        break;
                }
            }
            channel.setMode('o', mode_bool);
            mode_str += "o";
        }
       else if (mode[i] == 'i')
        {
            channel.setMode('i', mode_bool);
            mode_str += "i";
        }
        else if (mode[i] == 't')
        {
            channel.setMode('t', mode_bool);
            mode_str += "t";
        }
        else if (mode[i] == 'l' && messages.size() > 3)
        {
            if (mode_bool && std::stoi(messages[3]) > 0 && std::stoi(messages[3]) < 1000)
            {
                channel.setMaxUsers(std::stoi(messages[3]));
                channel.setMode('l', true);
            }
            else
            {
                channel.setMode('l', false);
                channel.setMaxUsers(1000);
            }
            mode_str += "l";
        }
        else if (mode[i] == 'k' && messages.size() > 3)
        {
            if (mode_bool && !messages[3].empty())
            {
                channel.setMode('k', true);
                channel.setPassword(messages[3]);
            }
            else
            {
                channel.setMode('k', false);
                channel.setPassword("");
            }
            mode_str += "k";
        }
        else
            return;
    }

    channel.sendMessage(":" + client.getNickName() + " MODE " + channel.getName() + " " + mode_str + "\r\n", "");
}


//Central command function
void executeCommand(std::vector<std::string> messages, User *user)
{
    // Get the command from the messages vector
    std::string message = messages[0];

    // Check if the message is a valid command
    if (!isCommand(message))
    {
        // Send an error message if the command is unknown
        irc::Server::serverInstance->sendMsg(user->getUserFd(), ":ASY 421, Unknown command\r\n");
        return;
    }

    // Handle general commands
    if (message == "NICK")
    {
        handle_nickname(user, messages);
        return;
    }
    else if (message == "PASS")
    {
        handle_pass(user, messages);
        return;
    }
    else if (message == "USER")
    {
        handle_user(user, messages);
        return;
    }
    else if (message == "CAP")
    {
        handle_cap(user, messages);
        return;
    }
    else if (message == "MOTD")
    {
        handle_motd(user);
        return;
    }

    // Get the channel object based on the message recipient
    Channel *channel = irc::Server::serverInstance->getChannel(messages[1]);

    // If the command is INVITE, retrieve the channel object from messages[2]
    if (message == "INVITE")
        channel = irc::Server::serverInstance->getChannel(messages[2]);

    // Check if the message is a WHOIS or MODE command targeting the FT_irc_server
    if (server_messages(messages) == true)
        return;

    // Handle channel-specific commands
    if (channel != nullptr)
    {
        if (message == "JOIN")
        {
            channel->join(user, messages);
        }
        else if (message == "PART")
        {
            channel->part(user, messages);
        }
        else if (message == "MODE")
        {
            channel->mode(user, messages);
        }
        else if (message == "TOPIC")
        {
            channel->topic(user, messages);
        }
        else if (message == "KICK")
        {
            channel->kick(user, messages);
        }
        else if (message == "INVITE")
        {
            channel->invite(user, messages);
        }
        else if (message == "PRIVMSG")
        {
            channel->privmsg(user, messages);
        }
    }
    else
    {
        irc::Server::serverInstance->sendMsg(user->getUserFd(), ":ASY 442, You're not on that channel\r\n");
    }
}
