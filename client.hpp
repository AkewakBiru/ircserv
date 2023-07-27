#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <string>

class Client
{
	public:
		Client();

	// msg format: USER username * * :full_name
	//             NICK nick
	private:
		std::string _nick;
		std::string _userName;
		std::string _fullName;
		bool _isRegistered;

};
#endif