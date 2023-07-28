/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 19:53:47 by abiru             #+#    #+#             */
/*   Updated: 2023/07/28 14:30:54 by abiru            ###   ########.fr       */
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

	// msg format: USER username * * :full_name
	//             NICK nick
	private:
		std::string _nick;
		std::string _userName;
		std::string _fullName;
		bool _isRegistered;
		int	_sockfd;

};
#endif