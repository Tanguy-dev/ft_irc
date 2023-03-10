/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 23:51:03 by thamon            #+#    #+#             */
/*   Updated: 2023/02/23 02:42:19 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <ctime>

bool						isLetter(char c);
bool						isSpecial(char c);
bool						isDigit(char c);

void						error(std::string message, bool stop);
bool						startWith(const std::string& str, const std::string& prefix);
std::vector<std::string>	split(std::string& s, std::string delimiter);
std::string					currentTime();


#endif