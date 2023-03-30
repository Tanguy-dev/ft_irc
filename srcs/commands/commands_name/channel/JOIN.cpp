/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 02:30:49 by thamon            #+#    #+#             */
/*   Updated: 2023/03/27 20:45:38 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"

void leaveAllChannels(Commands *command)
{
	std::vector<Channel *> channels = command->getServer().getChannels();
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		(*it)->sendMessage(&command->getUser(), "PART " + (*it)->getName() + (command->getParams().size() > 1 ? " :" + command->getParams()[1] : ""));
		(*it)->quit(&command->getUser());
		if ((*it)->getMembers().size() == 0)
			command->getServer().delChannel(*(*it));
	}
}

std::string getUsersInfo(Channel channel)
{
	std::vector<User *> users = channel.getMembers();
	std::string usersInfo = "";

	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (usersInfo.length())
			usersInfo += " ";
		if (channel.getUserMode(*(*it)).find('o') != std::string::npos)
			usersInfo += "@";
		usersInfo += (*it)->getNickname();
	}
	return (usersInfo);
}

void JOIN(Commands *command)
{
	if (command->getParams().size() == 0)
		return (command->rpl(461, "JOIN"));
	if (command->getParams()[0] == "0")
		return (leaveAllChannels(command));

	std::vector<std::string> channelsNames = split(command->getParams()[0], ",");
	std::vector<std::string> keys = command->getParams().size() > 1 ? split(command->getParams()[1], ",") : std::vector<std::string>();
	std::vector<std::string>::iterator it_keys = keys.begin();

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
			channel->setUserMode(command->getUser(), "o");
		}
		else
		{
			std::string key = it_keys != keys.end() ? *it_keys++ : "";

			if (channel->getMode().find('k') != std::string::npos && channel->getKey() != key)
			{
				command->rpl(475, *it);
				continue;
			}
			if (channel->getMode().find('l') != std::string::npos && channel->getMembers().size() >= (size_t)channel->getMaxUsers())
			{
				command->rpl(471, *it);
				continue;
			}
			if (channel->getMode().find('i') != std::string::npos && !channel->isInvited(&command->getUser()) && command->getUser().getMode().find('O') == std::string::npos)
			{
				command->rpl(473, *it);
				continue;
			}
			if (channel->isUserBanned(&command->getUser()))
			{
				command->rpl(474, *it);
				continue;
			}

			channel->join(&command->getUser());
			channel->removeInvited(&command->getUser());
		}

		std::string channelMode;

		if (channel->getMode().find('p') != std::string::npos)
			channelMode = "*";
		else if (channel->getMode().find('s') != std::string::npos)
			channelMode = "@";
		else
			channelMode = "=";

		if (channel->getTopic().length())
			command->rpl(332, *it, channel->getTopic());

		command->rpl(353, channelMode, *it, getUsersInfo(*channel));
		command->rpl(366, *it);
		channel->sendMessage(&command->getUser(), "JOIN :" + channel->getName());
		command->getUser().setChannel(channel->getName());
	}
}
