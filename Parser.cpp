/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 23:26:47 by abiru             #+#    #+#             */
/*   Updated: 2023/08/12 00:09:04 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(): _res(0)
{}

Parser::~Parser()
{}

std::vector<std::string> const &Parser::getRes(void) const
{
	return (_res);
}

bool Parser::isSpaces(std::string const &str)
{
	for (std::string::const_iterator i=str.begin(); i!=str.end(); ++i)
	{
		if (!std::isspace(*i))
			return (false);
	}
	return (true);
}

bool Parser::parseInput(char const *data)
{
	if (!data)
		return (false);
	std::string tmp = data;
	std::string params;
    std::string word;

	if (findMsgSize(data) > 0)
		tmp = tmp.substr(0, findMsgSize(data));
	std::stringstream ss(tmp);

    while (ss >> word) {
		if (_res.size() == 0)
		{
			if (word[0] == '!')
				_res.push_back(word);
			else
				_res.push_back("");
		}
		if (_res.size() < 2)
			_res.push_back(word);
		else
			params.append(word).append(" ");
    }
	params = params.substr(0, params.find_last_of(" "));
	_res.push_back(params);
	return (true);
}

void Parser::resetRes(void)
{
	_res.clear();
}

size_t Parser::findMsgSize(char const *data)
{
	if (!data)
		return (0);
	if (std::strchr(data, '\n'))
		return (std::strchr(data, '\n') - data);
	return (std::strchr(data, '\0') - data);
}