/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 23:26:47 by abiru             #+#    #+#             */
/*   Updated: 2023/07/27 23:43:05 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(): _res(0)
{}

Parser::~Parser()
{}

std::vector<std::string> Parser::getRes(void) const
{
	return (_res);
}

bool Parser::parseInput(char const *data)
{
	std::stringstream stream(data);
	std::string val;
	
	while (std::getline(stream, val, ' '))
		_res.push_back(val);
	return (true);
}