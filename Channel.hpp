/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjadalla <sjadalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 20:34:42 by abiru             #+#    #+#             */
/*   Updated: 2023/08/19 18:34:04 by sjadalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Client.hpp"
#include <vector>
#include <map>
#include <string>

class Client;

class Channel
{
    public:
        Channel(const std::string& _name);
        ~Channel();
        
        std::vector<Client *> *getMembers(void);
        void addUser(Client* user);
        void removeUser(Client* user);
        std::string getName() const;
        void setTopic(const std::string& newTopic);
        std::string getTopic() const;
        void setMode(char modeKey, bool value);
        bool getMode(char modeKey) const;
        void setPassword(const std::string& password);
        std::string getPassword() const;
        void setMaxUsers(int count);
        int getMaxUsers() const;

    private:
        std::string name;
        std::map<char, bool> mode;
        int max_users;
        std::string topic;
        std::string pwd;
        std::vector<Client *> _members;
        Client *_operator;
};

#endif
