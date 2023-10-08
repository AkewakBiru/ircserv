/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youssef <youssef@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 20:34:42 by abiru             #+#    #+#             */
/*   Updated: 2023/10/08 18:19:38 by youssef          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include "Client.hpp"

class Client;

class Channel
{
    private:
        std::string name;
        std::map<char, bool> mode;
        int max_users;
        std::string topic;
        std::string pwd;
        std::vector<Client *> _members;
        std::vector<Client *> _operators;
        static std::map<std::string, Channel*> allChannels;

		std::vector<int> _invitees;

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
        
        void kickUser(Client* _operator, Client* userToKick);
        void inviteUser(Client* inviter, Client* invitee);
        std::string execTopic(Client* requester, const std::string& newTopic = "");
        void execMode(Client* requester, char modeKey, bool value = false);
        static Channel* getChannel(const std::string& channelName);

		std::vector<Client *> const getOperators() const;
		void addOperator(Client *client);
        void removeOperaor(Client* client);
        
        bool isMember(Client* client);
        bool isOperator(Client* client);

		void addInvitee(Client *invitee);
		std::vector<int> const &getInvitees() const;

		bool isInvited(Client *client);
        void setOperatorStatus(Client* client, bool isOperator);
        
};

#endif
