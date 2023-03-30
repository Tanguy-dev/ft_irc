/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NOTICE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 14:21:05 by gadeneux          #+#    #+#             */
/*   Updated: 2023/03/16 02:03:33 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"
#include <string>

void NOTICE(Commands *command)
{
	// If no parameter is given for the command, return without doing anything
	if (command->getParams().empty())
		return;

	// If no message is given, return without doing anything
	if (command->getContent().empty())
		return;

	// Determine the target of the message (a user or a channel)
	std::string target = command->getParams()[0];
	std::vector<User *> targetUser;

	// If the target begins with a '#' character, it's a channel
	if (target[0] == '#')
	{
		// Check if the channel exists
		if (command->getServer().isChannel(target))
		{
			// Get the channel object
			Channel *channel = command->getServer().getChannel(target);
			// Get the list of members in the channel
			targetUser = channel->getMembers();
			// Remove the sender of the message from the list of recipients
			std::vector<User *>::iterator it = std::find(targetUser.begin(), targetUser.end(), &command->getUser());
			if (it != targetUser.end())
				targetUser.erase(it);
		}
		else
		{
			// If the channel does not exist, return without doing anything
			return;
		}
	}
	else
	{
		// If the target is not a channel, it's a user
		User *user = command->getServer().getUserByNickname(target);
		if (user)
			targetUser.push_back(user);
		else
			// If the user does not exist, return without doing anything
			return;
	}

	// Send the message to all the recipients
	for (std::vector<User *>::iterator it = targetUser.begin(); it != targetUser.end(); ++it)
		command->getUser().sendMessage(*(*it), "NOTICE " + target + " :" + command->getContent());
}
