/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KILL.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 00:28:50 by thamon            #+#    #+#             */
/*   Updated: 2023/03/22 00:47:23 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"

void KILL(Commands *command)
{
	if (command->getParams().size() < 2)
		return (command->rpl(461, "KILL"));

	if (!command->getUser().isAdmin())
		return (command->rpl(481));

	User *target = command->getServer().getUser(command->getParams()[0]);
	if (target == NULL)
		return (command->rpl(401, command->getParams()[0]));

	std::string reason = command->getParams().size() >= 2 ? command->getContent() : "";

	// Envoyer un message à la targete avant de la déconnecter
	target->sendMessage(*target, "KILL " + target->getNickname() + " :" + reason);

	// Déconnecter la targete du serveur
	target->setStatus(DELETE);

	// Envoyer une réponse à l'opérateur qui a lancé la commande KILL
	command->getUser().sendMessage(command->getUser(), target->getNickname() + " has been disconnected");
}
