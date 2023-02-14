/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getParams.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 23:30:34 by thamon            #+#    #+#             */
/*   Updated: 2023/02/04 00:10:49 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "getParams.hpp"

GetParams::GetParams() {
	;
}

void		GetParams::set(std::string name, std::string value) {
	values[name] = value;
	std::cout << values[name] << std::endl;
}

std::string	GetParams::get(std::string name) {
	return values[name];
}