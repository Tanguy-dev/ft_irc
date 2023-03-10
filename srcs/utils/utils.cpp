/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 23:51:07 by thamon            #+#    #+#             */
/*   Updated: 2023/02/23 02:48:21 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

bool	isLetter(char c) { return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')); }
bool	isSpecial(char c) { return ((c >= '[' && c <= '`') || (c >= '{' && c <= '}')); }
bool	isDigit(char c) { return ((c >= '0' && c <= '9')); }

void	error(std::string message, bool stop)
{
	std::cout << "error : " << message << std::endl;
	if (stop)
		exit(EXIT_FAILURE);
}

bool	startWith(const std::string& str, const std::string& prefix) {
    return (str.substr(0, prefix.length()) == prefix);
}

std::vector<std::string>	split(std::string& s, std::string delimiter) {
	std::vector<std::string> values = std::vector<std::string>();

	size_t position;
	while ((position = s.find(delimiter)) != std::string::npos)
	{
		values.push_back(s.substr(0, position));
		s.erase(0, position + delimiter.length());
	}
	values.push_back(s);

	return (values);
}

std::string currentTime()
{
	time_t t = std::time(0);
	struct tm *now = std::localtime(&t);
	std::string time(asctime(now));
	time.erase(--time.end());
	return (time);
}
