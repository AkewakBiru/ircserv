/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 23:26:47 by abiru             #+#    #+#             */
/*   Updated: 2023/08/18 15:47:07 by abiru            ###   ########.fr       */
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

/*
	** splits msg using 0x20 and puts it in a vector
*/
void Parser::parseInput(std::string arg)
{
	size_t start = 0, end=0;

	rtrim(arg, "\r\n");
	if (arg.find_first_of("\v\n\r") != std::string::npos)
		throw std::invalid_argument(genErrMsg(ERR_UNKNOWNCOMMAND, "*", "*", ERR_UNKNOWNCOMMAND_ILLEGAL));
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
