/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 01:00:23 by thamon            #+#    #+#             */
/*   Updated: 2023/04/03 05:28:43 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"

void NICK(Commands *command)
{
	// Si aucun nickname est envoyer on envoi "ERR_NONICKNAMEGIVEN"
	if (!command->getParams().size())
		return command->rpl(431);

	std::string nickname = command->getParams()[0];


	// Si la size du nickname est > 9 on envoi "ERR_ERRONEUSNICKNAME"
	if (nickname.length() >= 29)
		return (command->rpl(432, nickname));

	// Si le premier characters de nickname est un chiffre on envoi "ERR_ERRONEUSNICKNAME"
	if (!isLetter(nickname[0]) && !isSpecial(nickname[0]))
		return (command->rpl(432, nickname));

	// Si nickname contien des invalid characters on envoi "ERR_ERRONEUSNICKNAME"
	for (unsigned int i = 0; i != nickname.length(); i++)
		if (!isLetter(nickname[i]) && !isSpecial(nickname[i]) && !isDigit(nickname[i]) && nickname[i] != '-')
			return (command->rpl(432, nickname));

	// Recupere les users
	std::vector<User*> users = command->getServer().getUsers();

	// Check si ne nickname n'est pas deja utilise si oui on envoi "ERR_NICKNAMEINUSE"
	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); it++)
		if (nickname == (*it)->getNickname())
			return (command->rpl(433, nickname));

	std::string message = "NICK :" + nickname;

	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		command->getUser().sendMessage(*(*it), message);
	}
	command->getUser().setNickname(nickname);
}