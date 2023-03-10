/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BAN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gadeneux <gadeneux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 14:21:05 by gadeneux          #+#    #+#             */
/*   Updated: 2023/03/09 15:03:59 by gadeneux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"

void BAN(Commands *command)
{
	Channel *chann = command->getServer().getChannel(command->getParams()[0]);
	std::string nickname = command->getParams()[1];
	int delay_in_seconds = atoi(command->getParams()[2].c_str());

	if (!chann)
	{
		// Le channel n'existe pas
		return (command->rpl(403));
	}

	if (!chann->isMember(&(command->getUser())))
	{
		// L'utilisateur n'est pas membre du channel
		return (command->rpl(442));
	}

	User * user = command->getServer().getUserByNickname(nickname);

	if (user == NULL)
		return (command->rpl(401));

	// if (!chann->isOperator(user))
	// {
	// 	// L'utilisateur n'a pas les privilèges pour bannir
	// 	return (command->rpl(482));
	// }

	chann->ban(user, delay_in_seconds);
}
