/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 02:30:49 by thamon            #+#    #+#             */
/*   Updated: 2023/03/16 22:24:24 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"

void PART(Commands *command)
{
	if (command->getParams().size() == 0)
	{
		command->rpl(461, "PART");
		return;
	}

	std::vector<std::string> channels = split(command->getParams()[0], ",");

	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		std::string &channel = *it;

		if (channel.size() == 0)
			continue;
		if (command->getServer().isChannel(channel))
		{
			Channel &chan = *command->getServer().getChannel(channel);
			if (!chan.isMember(&command->getUser()))
			{
				command->rpl(442, channel);
				continue;
			}
			chan.sendMessage(&command->getUser(), "PART " + channel + (command->getContent().size() ? " :" + command->getContent() : ""));
			chan.quit(&command->getUser());
			if (chan.getMembers().size() == 0)
				command->getServer().delChannel(chan);
		}
		else
			command->rpl(403, channel);
	}
}
