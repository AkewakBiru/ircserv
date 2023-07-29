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

