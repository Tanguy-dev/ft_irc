/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 14:21:05 by gadeneux          #+#    #+#             */
/*   Updated: 2023/03/30 19:08:10 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"
#include <sstream>
#include <iostream>

void MODE(Commands *command)
{
	if (command->getParams().size() == 0)
		return (command->rpl(461, "MODE"));

	if (command->getParams().size() >= 2 && command->getParams()[0][0] != '#' && (command->getParams()[1] == "+i" || command->getParams()[1] == "-i"))
	{
		if (command->getUser().getNickname() == command->getParams()[0])
		{
			User *user = command->getServer().getUser(command->getParams()[0]);
			if (command->getParams()[1] == "+i")
			{
				user->setMode('i', true);
				return (command->rpl(*user, 221, "+i"));
			}
			else if (command->getParams()[1] == "-i")
			{
				user->setMode('i', false);
				return (command->rpl(*user, 221, "-i"));
			}
		}
		return;
	}
	if (command->getServer().isChannel(command->getParams()[0]) == false)
	{
		// Le channel n'existe pas
		return (command->rpl(403, command->getParams()[0]));
	}

	Channel *chann = command->getServer().getChannel(command->getParams()[0]);

	if (!chann->isMember(&(command->getUser())))
	{
		// L'utilisateur n'est pas membre du channel
		return (command->rpl(442));
	}

	if (command->getParams().size() > 1)
	{
		std::string modes = command->getParams()[1];
		if (modes.empty() || (modes[0] != '+' && modes[0] != '-'))
		{
			// Le type est invalide
			return (command->rpl(461, "MODE"));
		}
		// Check si l'utilisateur a les privilèges operateur
		if (!command->getUser().isAdmin() && chann->getUserMode(command->getUser()).find('o') == std::string::npos)
		{
			// L'utilisateur n'a pas les privilèges operateur
			return (command->rpl(482, command->getParams()[0]));
		}

		if (modes == "+b")
		{
			if (command->getParams().size() == 3 || command->getParams().size() == 4)
			{
				User *user = command->getServer().getUserByPrefix(command->getParams()[2]);
				int delay_in_seconds = 0;
				if (command->getParams().size() == 3)
					delay_in_seconds = 2147483647;
				else
					delay_in_seconds = atoi(command->getParams()[3].c_str());

				if (user == NULL)
				{
					// L'utilisateur n'est pas présent sur le serveur
					return (command->rpl(401, command->getParams()[2]));
				}

				if (chann->ban(user, delay_in_seconds))
					/* Send feedback message from server to client */;
			}
		}
		else if (modes == "-b")
		{
			if (command->getParams().size() == 3)
			{
				User *user = command->getServer().getUserByPrefix(command->getParams()[2]);

				if (user == NULL)
				{
					// L'utilisateur n'est pas présent sur le serveur
					return (command->rpl(401));
				}

				if (chann->unban(user))
					/* Send feedback message from server to client */;
			}
		}
		else if (modes == "+o")
		{
			if (command->getParams().size() == 3)
			{
				User *user = command->getServer().getUserByNickname(command->getParams()[2]);

				if (user == NULL)
				{
					// L'utilisateur n'est pas présent sur le serveur
					return (command->rpl(401));
				}
				chann->setUserMode(*user, "o");
			}
		}
		else if (modes == "-o")
		{
			if (command->getParams().size() == 3)
			{
				User *user = command->getServer().getUserByNickname(command->getParams()[2]);

				if (user == NULL)
				{
					// L'utilisateur n'est pas présent sur le serveur
					return (command->rpl(401));
				}
				chann->setUserMode(*user, "");
			}
		} else {
			bool neg = false;
			int params = 99;
			if (command->getParams().size() >= 3)
				params = 2;
			for (size_t j = 0; j < modes.length(); j++)
			{
				char mode = modes[j];

				if (mode == '-')
					neg = true;
				else if (mode == '+')
					neg = false;
				if (mode == '-' || mode == '+')
					continue ;

				if (mode == 'i' && neg == false)
					chann->setMode('i', true);
				else if (mode == 'i')
					chann->setMode('i', false);
				else if (mode == 'k' && neg == false)
				{
					if (command->getParams().size() + 1 >= (size_t)params)
					{
						std::string key = command->getParams()[params++];

						chann->setMode('k', true);
						chann->setKey(key);
					}
					else
					{
						chann->setMode('k', false);
						chann->setKey("");
					}
				}
				else if (mode == 'k')
				{
					chann->setMode('k', false);
					chann->setKey("");
				}
				else if (mode == 'l' && neg == false)
				{
					if (command->getParams().size() + 1 >= (size_t)params)
					{
						int max_users = std::atoi(command->getParams()[params++].c_str());

						chann->setMode('l', true);
						chann->setMaxUsers(max_users);
					}
					else
					{
						chann->setMode('l', false);
						chann->setMaxUsers(0);
					}
				}
				else if (mode == 'l')
				{
					chann->setMode('l', false);
					chann->setMaxUsers(0);
				}
			}
		}
	}
	std::string mode = command->getServer().getChannel(command->getParams()[0])->getMode();

	if (chann->getUserMode(command->getUser()).find('o') != std::string::npos || command->getUser().getMode().find("O") != std::string::npos)
	{
		std::string options = command->getServer().getChannel(command->getParams()[0])->getKey();
		if (options.size() > 0)
			options += " ";
		if (command->getServer().getChannel(command->getParams()[0])->getMaxUsers() != 0)
			options += static_cast<std::ostringstream*>( &(std::ostringstream() << command->getServer().getChannel(command->getParams()[0])->getMaxUsers()) )->str();
		return (command->rpl(324, command->getParams()[0], "+" + mode, options));
	}
	return (command->rpl(324, command->getParams()[0], "+" + mode, ""));
}
