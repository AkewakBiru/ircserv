/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 23:24:49 by abiru             #+#    #+#             */
/*   Updated: 2023/07/28 11:44:50 by abiru            ###   ########.fr       */
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
		bool parseInput(char const *data, std::string const &delim);

	private:
		std::vector<std::string> _res;

};
#endif