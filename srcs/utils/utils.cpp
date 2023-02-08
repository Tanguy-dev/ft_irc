/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 23:51:07 by thamon            #+#    #+#             */
/*   Updated: 2023/02/03 23:54:26 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void error(std::string message, bool stop)
{
	std::cout << "error : " << message << std::endl;
	if (stop)
		exit(EXIT_FAILURE);
}