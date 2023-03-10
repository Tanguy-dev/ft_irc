/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 14:21:05 by gadeneux          #+#    #+#             */
/*   Updated: 2023/03/10 03:01:53 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"
#include <string>

#define TARGET_USER 1
#define TARGET_CHAN 2
#define TARGET_USCH 3

void PRIVMSG(Commands *command)
{
	if (!command->getParams().size())
		return command->rpl(411);
	if (!command->getContent().length())
		return command->rpl(412);

	std::string target = command->getParams()[0];
	std::vector<User *> targetUser;


	// // Trouver l'indice du premier ':' dans les arguments de la commande
	// size_t colon_index = command_string.find_first_of(':');

	// // S'il n'y a pas de ':', signaler une erreur de syntaxe
	// if (colon_index == std::string::npos)
	// 	return (command->rpl(501));

	// // Déterminer combien d'arguments il y a avant le ':'
	// size_t num_args_before_colon = std::count(command_string.begin(), command_string.begin() + colon_index, ' ') + 1;

	// Déterminer le type de cible (User, channel ou User + channel)
	// size_t tartype = 0;

	// if (num_args_before_colon - 1 == 2)
	// 	tartype = TARGET_USCH;
	// else if (num_args_before_colon == 1)
	// 	tartype = (params[0].front() == '#' ? TARGET_CHAN : TARGET_USER);

	if (*target.begin() == '#') {
		if (command->getServer().isChannel(target)) {
			command->getUser().sendMessage(command->getUser(), "bien");
			Channel *chann = command->getServer().getChannel(target);

			// Ici eventuellement on peut mettre s'il possède une exception
			// de ban (genre un operator etc.) mais c'est pas demandé dans le sujet

			// bool user_is_covered_by_ban_exception = false;

			// if (chann->isUserBanned(command->getUser().getNickname()) && !user_is_covered_by_ban_exception)
			// {
			// 	// L'utilisateur est banni du channel
			// 	return (command->rpl(404));
			// }

			// Envoi du message au channel 
			targetUser = chann->getMembers();
			std::vector<User *>::iterator it = std::find(targetUser.begin(), targetUser.end(), &command->getUser());
			if (it != targetUser.end())
				targetUser.erase(it);
		} else 
			return (command->rpl(404, target));
	} else {
		if (command->getServer().getUserByNickname(target))
			targetUser.push_back(command->getServer().getUserByNickname(target));
		else
			return command->rpl(401, target);
	}

	for (std::vector<User *>::iterator it = targetUser.begin(); it != targetUser.end(); ++it)
		command->getUser().sendMessage(*(*it), "PRIVMSG " + target + " :" + command->getContent());
}