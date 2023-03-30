/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OPER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 20:16:22 by gadeneux          #+#    #+#             */
/*   Updated: 2023/03/23 01:36:31 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"
#include <string>

void OPER(Commands *command)
{
	if (command->getParams().size() < 2)
		return (command->rpl(461, "OPER")); // Not enough parameters

	std::string username = command->getParams()[0];
	std::string password = command->getParams()[1];

	if (command->getServer().getConfig().get("admin") == username && command->getServer().getConfig().get("adminPassword") == password)
	{
		// L'utilisateur est un opérateur, donner les privilèges
		command->getUser().setMode('O', true);
		command->rpl(381);
	}
	else if (command->getServer().getConfig().get("admin") == username && command->getServer().getConfig().get("adminPassword") != password)
	{
		command->rpl(464);
	}
	else
		command->rpl(491);
}