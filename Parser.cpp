/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 23:26:47 by abiru             #+#    #+#             */
/*   Updated: 2023/07/28 12:08:08 by abiru            ###   ########.fr       */
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

bool Parser::parseInput(char const *data, std::string const &delim)
{
	std::stringstream stream(data);
	std::string val;

	while (std::getline(stream, val))
	{
		size_t start = 0;
		size_t end = val.find_first_of(delim);

		while (end != std::string::npos)
		{
			if (end != start)
				_res.push_back(val.substr(start, end - start));
			start = end + 1;
			end = val.find_first_of(delim, start);
		}

		if (start < val.length())
			_res.push_back(val.substr(start));
	}
	return (true);
}

void Parser::resetRes(void)
{
	_res.clear();
}