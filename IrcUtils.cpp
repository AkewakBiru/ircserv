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
