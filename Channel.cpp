/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjadalla <sjadalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 21:07:13 by abiru             #+#    #+#             */
/*   Updated: 2023/08/19 18:47:11 by sjadalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <algorithm>

Channel::Channel(const std::string& _name) : name(_name), mode(), max_users(1000),
      topic(""), pwd(""), _members(), _operator(nullptr)
{
    mode['o'] = false;  // channel operator
    mode['k'] = false;  // channel password (key)
    mode['t'] = false;  // topic settable by channel operator only
    mode['i'] = false;  // invite-only channel
    mode['l'] = false;  // channel has user limit
}

Channel::~Channel() {}

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
