/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 19:53:47 by abiru             #+#    #+#             */
/*   Updated: 2023/08/11 17:30:59 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <string>
#include <unistd.h>

class Client
{
	public:
		Client();
		~Client();

		void setFd(int fd);
		int getFd() const;

		void setNick(std::string const &nick);
		std::string const &getNick(void) const;
		
		void setUserName(std::string const &user);
		std::string const &getUserName() const;

		void setJoinedTime(unsigned long time);
		unsigned long getJoinedTime() const;

		void setStatus(bool status);
		bool getStatus() const;

		void setPassword(bool value);
		bool hasPassword() const;

	// msg format: USER username * * :full_name
	//             NICK nick
	private:
		std::string _nick;
		std::string _userName;
		std::string _fullName;
		bool _isRegistered;
		bool _password;
		int	_sockfd;
		std::string _buffer;
		char _msg[1024];
		unsigned long _joinedTime;
};
#endif