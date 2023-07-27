/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 23:24:49 by abiru             #+#    #+#             */
/*   Updated: 2023/07/27 23:36:22 by abiru            ###   ########.fr       */
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
		std::vector<std::string> getRes(void) const;
		bool parseInput(char const *data);

	private:
		std::vector<std::string> _res;

};
#endif