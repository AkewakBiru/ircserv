/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 23:24:49 by abiru             #+#    #+#             */
/*   Updated: 2023/08/16 20:59:28 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP
#include <iostream>
#include <vector>
#include <sstream>
#include "IrcUtils.hpp"

class Parser
{
	public:
		Parser();
		~Parser();
		std::vector<std::string>const &getRes(void) const;
		void resetRes(void);
		void parseInput(std::string data);
		size_t findMsgSize(char const *data);

	private:
		std::vector<std::string> _res;
};
#endif