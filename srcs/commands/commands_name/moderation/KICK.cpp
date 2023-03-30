/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 20:16:22 by gadeneux          #+#    #+#             */
/*   Updated: 2023/03/27 16:53:57 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"
#include <string>

void KICK(Commands *command)
{
	if (command->getParams().size() < 2)
		return (command->rpl(461, "KICK")); // Not enough parameters

	std::string chann_name = command->getParams()[0];
	std::string user_to_kick_name = command->getParams()[1];

	// Vérifie que le channel existe
	if (!command->getServer().isChannel(chann_name))
		return (command->rpl(403, chann_name)); // No such channel

	Channel *channel = command->getServer().getChannel(chann_name);

	// Regarder si l'utilisateur existe
	if (!channel->isMemberName(user_to_kick_name))
		return (command->rpl(401, user_to_kick_name)); // No such nick/channel

	User *userToKick = command->getServer().getUserByNickname(user_to_kick_name);

	// Vérifie que l'utilisateur est opérateur
	if (!command->getUser().isAdmin() && channel->getUserMode(command->getUser()).find('o') == std::string::npos)
		return (command->rpl(482, chann_name)); // You're not channel operator
	
	// Envoie du message de kick
	channel->sendMessage(&command->getUser(), "KICK " + chann_name + " " + user_to_kick_name + " :" + command->getParams()[2]);
	
	// Retire l'utilisateur du channel
	channel->quit(userToKick);
}