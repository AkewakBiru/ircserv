/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 23:26:47 by abiru             #+#    #+#             */
/*   Updated: 2023/08/16 21:04:54 by abiru            ###   ########.fr       */
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

void Parser::parseInput(std::string arg)
{
	int i = 0, start = 0, end=0;

	rtrim(arg, "\r\n");		
	while (start < arg.length())
	{
		while (arg[start] == ' ')
			start++;
		if (_res.size() == 0)
		{
			if (arg[start] == ':')
			{
				end = arg.find_first_of(" ", start);
				if (end == start)
				{
					start++;
					continue ;
				}
				if (end == std::string::npos)
				{
					_res.push_back(arg.substr(start, arg.length() - start));
					break ;
				}
			}
			else
				_res.push_back("");
		}
		end = arg.find_first_of(" ", start);
		if (end == start)
		{
			start++;
			continue ;
		}
		if (end == std::string::npos)
		{
			_res.push_back(arg.substr(start, arg.length() - start));
			break ;
		}
		_res.push_back(arg.substr(start, end - start));
		while (arg[end] == ' ')
			end++;
		start = end;
	}
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