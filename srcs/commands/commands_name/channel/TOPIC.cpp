/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 00:12:05 by thamon            #+#    #+#             */
/*   Updated: 2023/03/22 00:28:57 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"

void TOPIC(Commands *command)
{
	if (command->getParams().size() == 0)
		return (command->rpl(461, "TOPIC"));

	std::string channelName = command->getParams()[0];

	if (!command->getServer().isChannel(channelName))
		return (command->rpl(403, channelName));

	Channel *channel = command->getServer().getChannel(channelName);

	if (command->getParams().size() == 1) // Afficher le sujet
	{
		if (channel->getTopic().empty())
			command->rpl(331, channelName);
		else
			command->rpl(332, channelName, channel->getTopic());
	}
	else // Modifier le sujet
	{
		if (!channel->isMember(&command->getUser()))
			return (command->rpl(442, channelName));

		if (channel->getUserMode(command->getUser()).find('O') != std::string::npos && channel->getUserMode(command->getUser()).find('O') != std::string::npos && channel->getUserMode(command->getUser()).find('o') != std::string::npos)
			return (command->rpl(482, channelName));

		std::string newTopic = command->getContent();
		channel->setTopic(newTopic);
		channel->sendMessage(&command->getUser(), "TOPIC " + channelName + " :" + newTopic);
	}
}
