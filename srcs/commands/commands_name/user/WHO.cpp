/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 00:29:13 by thamon            #+#    #+#             */
/*   Updated: 2023/03/27 20:51:35 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"

void WHO(Commands *command)
{
	if (command->getParams().size() == 0)
		return ;

	std::vector<User *> users = command->getServer().getUsers();

	if (command->getParams()[0][0] == '#')
	{
		if (command->getServer().isChannel(command->getParams()[0]) == false)
			return (command->rpl(315, command->getUser().getUsername()));

		Channel *channel = command->getServer().getChannel(command->getParams()[0]);

		for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
		{
			if (channel->isMember((*it)))
			{
				if (command->getUser().getMode().find('o') == std::string::npos && (*it)->getMode().find('i') != std::string::npos)
					continue;
				std::string flags;
				flags = "H";
				if ((*it)->getMode().find('o') != std::string::npos)
					flags += "@";
				command->rpl(352, (*it)->getChannel(), (*it)->getUsername(), (*it)->getHostname(), "", (*it)->getNickname(), flags, (*it)->getRealname());
			}
		}
		return (command->rpl(315, command->getUser().getUsername()));
	}
	
	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (command->getUser().getMode().find('o') == std::string::npos && (*it)->getMode().find('i') != std::string::npos)
			continue;
		std::string flags;
		flags = "H";
		if ((*it)->getMode().find('o') != std::string::npos)
			flags += "@";
		command->rpl(352, (*it)->getChannel(), (*it)->getUsername(), (*it)->getHostname(), "", (*it)->getNickname(), flags, (*it)->getRealname());
	}
	return (command->rpl(315, command->getUser().getUsername()));
}
