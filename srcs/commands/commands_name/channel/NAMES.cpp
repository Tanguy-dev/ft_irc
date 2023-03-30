/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NAMES.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 18:54:40 by thamon            #+#    #+#             */
/*   Updated: 2023/03/27 20:46:35 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"

void NAMES(Commands *command)
{
	if (command->getParams().size() == 0)
		return (command->rpl(461, "NAMES"));

	std::string channelName = command->getParams()[0];

	if (!command->getServer().isChannel(channelName))
		return (command->rpl(403, channelName));

	Channel *channel = command->getServer().getChannel(channelName);

	std::vector<User *> users = channel->getMembers();

	std::string usersInfo = "";

	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (usersInfo.length())
			usersInfo += " ";
		if (channel->getUserMode(*(*it)).find('o') != std::string::npos)
			usersInfo += "@";
		usersInfo += (*it)->getNickname();
	}

	command->rpl(353, "=", channelName, usersInfo);
	command->rpl(366, channelName);
}