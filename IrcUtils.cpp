#include "IrcUtils.hpp"

bool hasWhiteSpace(std::string const &str)
{
	for (size_t i=0; i<str.length(); i++)
	{
		if (std::isspace(str[i]))
			return (true);
	}
	return (false);
}

std::string toUpper(std::string const &arg, bool flag)
{
	std::string newStr(arg);
	if (arg.size() == 0)
		return ("");
	for (size_t i=0; i<newStr.length(); i++)
	{
		if (newStr[i] >= 'a' && newStr[i] <= 'z')
			newStr[i] = newStr[i] - ' ';
		else if (flag)
		{
			if (newStr[i] == '{')
				newStr[i] = '[';
			else if (newStr[i] == '}')
				newStr[i] = ']';
			else if (newStr[i] == '|')
				newStr[i] = '\\';
			else if (newStr[i] == '^')
				newStr[i] = '~';
		}
	}
	return (newStr);
}

std::string genErrMsg(char const *errCode, std::string optional, std::string cmd, std::string desc)
{
	std::string error = ":ircserv ";
	error.append(errCode).append(" ").append(optional).append(" ").append(cmd).append(" ").append(desc).append("\r\n");
	return (error);
}

std::string genServErrMsg(std::string nick, std::string ip, std::string desc)
{
	std::string error = "ERROR :Closing Link: ";
	error.append(nick).append("[").append(ip).append("]").append(" (").append(desc).append(")\r\n");
	return (error);
}