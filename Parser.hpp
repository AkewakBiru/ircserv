/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 23:24:49 by abiru             #+#    #+#             */
/*   Updated: 2023/08/12 22:08:58 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP
#include <iostream>
#include <vector>
#include <sstream>

class Parser
{
	public:
		Parser();
		~Parser();
		std::vector<std::string>const &getRes(void) const;
		void resetRes(void);
		bool isSpaces(std::string const &str);
		bool parseInput(std::string data);
		size_t findMsgSize(char const *data);

	private:
		std::vector<std::string> _res;
};
#endif