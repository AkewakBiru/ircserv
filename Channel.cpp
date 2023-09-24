/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 21:07:13 by abiru             #+#    #+#             */
/*   Updated: 2023/09/24 14:39:01 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <algorithm>

Channel::Channel(const std::string& _name) : name(_name), mode(), max_users(1000),
      topic(""), pwd(""), _members(), _operators(0)
{
    // Initialize channel modes to false
    mode['o'] = false;  // channel operator
    mode['k'] = false;  // channel password (key)
    mode['t'] = false;  // topic settable by channel operator only
    mode['i'] = false;  // invite-only channel
    mode['l'] = false;  // channel has user limit
}

Channel::~Channel() {}

// Function to get the list of members in the channel
std::vector<Client *> *Channel::getMembers(void)
{
    return &_members;
}

void Channel::addUser(Client* user) 
{
    _members.push_back(user);
}

void Channel::removeUser(Client* user) 
{
    _members.erase(std::remove(_members.begin(), _members.end(), user), _members.end());
}

std::string Channel::getName() const 
{
    return name;
}

void Channel::setTopic(const std::string& newTopic) 
{
    topic = newTopic;
}

std::string Channel::getTopic() const 
{
    return topic;
}

void Channel::setMode(char modeKey, bool value) 
{
    mode[modeKey] = value;
}

bool Channel::getMode(char modeKey) const 
{
    if (mode.find(modeKey) != mode.end())
        return mode.at(modeKey);
    return false; // Default if modeKey not found
}


void Channel::setPassword(const std::string& password) 
{
    pwd = password;
}

std::string Channel::getPassword() const 
{
    return pwd;
}

void Channel::setMaxUsers(int count) 
{
    max_users = count;
}

int Channel::getMaxUsers() const 
{
    return max_users;
}

// Static map to hold all channel instances
std::map<std::string, Channel*> Channel::allChannels;

// Static function to get a channel by its name
Channel* Channel::getChannel(const std::string& channelName) {
    if (allChannels.find(channelName) != allChannels.end()) {
        return allChannels[channelName];
    }
    return nullptr;// Return null if the channel is not found
}

std::vector<Client *> const Channel::getOperators() const {
	return (_operators);
}

void Channel::addOperator(Client *client) {
	_operators.push_back(client);
}