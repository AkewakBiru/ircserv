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

