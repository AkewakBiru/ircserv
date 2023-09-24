#include "channel_commands.hpp"
#include "Server.hpp"  

ChannelCommands::ChannelCommands() {}

ChannelCommands::~ChannelCommands() {}

void ChannelCommands::invite(Channel& channel, Client& inviter, Client& invitee)
{
    // Check if the channel is in invite-only mode and if the inviter is not an operator
    if (channel.getMode('i') && !inviter.is_op(channel))
        return;

    // Get the nickname of the invitee
    std::string nick = invitee.getNickName();

    // Loop through all clients connected to the server
    for (auto& user : Server::getClients())
    {
        // If the nickname matches, set the invite flag and send a message
        if (user->getNickName() == nick)
        {
            user->setInvited(channel, true);
            // Use the Server's sendToClient function to send the message
            Server::sendToClient(user->getUserFd(), "You have been invited to the channel.");
            break;
        }
    }
}


void ChannelCommands::kick(Channel& channel, Client& kicker, Client& kickee)
{
    // Get the nickname of the user to be kicked
    std::string nick = kickee.getNickName();

    // Check if the kicker is an operator
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
             Server::sendToClient(user->getUserFd(), "You have been kicked from the channel.");
            channel.removeUser(user); 
            break;
        }
    }
}


void ChannelCommands::setTopic(Channel& channel, Client& client, const std::vector<std::string>& messages)
{

    // Validate the input messages
     if (messages.size() < 3 || channel.getName() != messages[1])
        return;

    // Check if the topic is being queried
    if (messages.size() == 2)
    {
        channel.sendMessage("IRC: 331 " + client.getNickName() + " " + channel.getName() + " " + channel.getTopic(), "");
        return;
    }

    // Check if the client is an operator and if the topic is operator-only
    if (channel.getMode('t') && !client.is_op(channel))
    {
        Server::sendToClient(client->getUserFd(), "Error: You are not a channel operator.");
        return;
    }

    // Extract the new topic from the messages
    unsigned int startIndex = (messages[2] == ":") ? 3 : 2;
    std::string topic = "";

    if (messages.size() > 3 || messages[2] != ":")
    {
        topic = messages[startIndex];
        for (unsigned int i = startIndex + 1; i < messages.size(); i++)
            topic += " " + messages[i];
    }

    // Set the new topic and send a message
    channel.setTopic(topic);
    channel.sendMessage("IRC: 332 " + client.getNickName() + " " + channel.getName() + " " + channel.getTopic(), "");

}

// Function to manage channel modes (e.g., operator, invite-only, etc.)
void ChannelCommands::manageMods(Channel& channel, Client& client, const std::vector<std::string>& messages)
{

    // Initialize a boolean to keep track of the mode being set (+) or unset (-)
    bool mode_bool = false;

    // Basic validation: Check if the client is an operator and if the channel name matches
    if (messages.size() < 2 || channel.getName() != messages[1] || !client.is_op(channel))
        return;

    // If only the channel name is provided, send the current modes of the channel
    if (messages.size() == 2)
    {
        //build mode_str
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

        // Send the current modes to the client
        Server::sendToClient(client.getUserFd(), "324 " + client.getNickName() + " " + channel.getName() + " +" + mode_str + "\r\n");
    }

    // Extract the mode string from the messages
    std::string mode = messages[2];
    std::string mode_str = "";

    // Validate the mode string
    if (mode.length() < 2 || (mode[0] != '+' && mode[0] != '-'))
        return;

    // Loop through the mode string to set or unset each mode
    for (unsigned int i = 0; i < mode.length(); i++)
    {
        //handle each mode
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
            // Loop through all clients to find the one with the matching nickname
            for (auto& user : Server::getClients()
            {
                 // Set or unset the operator status for the user in the channel
                if (user->getNickName() == nick)
                {
                    user->setChannelOp(channel, mode_bool);
                    if (user->is_op(channel))
                        break;
                }
            }
            // Update the channel's mode
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
            // Check if the limit is within a valid range
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
            // Check if a password is provided
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
    // Send a message to the channel indicating the mode change
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
        Server::sendToClient(user->getUserFd(), ": 421, Unknown command\r\n");
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
     Channel *channel = Channel::getChannel(messages[1]);

    // If the command is INVITE, retrieve the channel object from messages[2]
    if (message == "INVITE")
         Channel *channel = Channel::getChannel(messages[2]);

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
        // If the channel is not found, send an error message to the client
        Server::sendToClient(user->getUserFd(), ": 442, You're not on that channel\r\n");
    }
}