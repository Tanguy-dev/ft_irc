/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 02:30:49 by thamon            #+#    #+#             */
/*   Updated: 2023/03/10 03:06:53 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"

void leaveAllChannels(Commands *command) {
	std::vector<Channel *> channels = command->getServer().getChannels();
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		(*it)->sendMessage(&command->getUser(), "PART " + (*it)->getName() + (command->getParams().size() > 1 ? " :" + command->getParams()[1] : ""));
		// (*it).(command->getUser());
		if ((*it)->getMembers().size() == 0)
			;
			// del channel
	}
}

std::string getUsersInfo(Channel channel) {
	std::vector<User *> users = channel.getMembers();
	std::string usersInfo = "";

	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (usersInfo.length())
			usersInfo += " ";
		usersInfo += (*it)->getNickname();
	}
	return (usersInfo);
}

void JOIN(Commands *command)
{
    if (command->getParams().size() == 0)
        return (command->rpl(461, "join"));
    if (command->getParams()[0] == "0")
        return (leaveAllChannels(command));
    std::vector<std::string> channelsNames = split(command->getParams()[0], ",");
    for (std::vector<std::string>::iterator it = channelsNames.begin(); it != channelsNames.end(); ++it)
    {
        if (it->c_str()[0] != '#')
        {
            command->rpl(476, *it);
            continue;
        }
        Channel *channel = command->getServer().getChannel(*it);
        if (channel->getMembers().size() == 0)
        {
            channel->join(&command->getUser());
        } else {
			channel->join(&command->getUser());
		}
        std::string channel_mode;
        channel_mode = "=";
        if (channel->getTopic().length())
            command->rpl(332, *it, channel->getTopic());
        command->rpl(353, channel_mode, *it, getUsersInfo(*channel));
        command->rpl(366, *it);
        channel->sendMessage(&command->getUser(), "JOIN :" + channel->getName());
    }
}
