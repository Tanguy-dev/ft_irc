/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 14:21:05 by gadeneux          #+#    #+#             */
/*   Updated: 2023/03/13 23:25:13 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"
#include <string>

void PRIVMSG(Commands *command)
{
	// If no parameter is given for the command, reply with error code 411 (no recipient given)
	if (!command->getParams().size())
		return command->rpl(411);
	// If no message is given, reply with error code 412 (no text to send)
	if (!command->getContent().length())
		return command->rpl(412);

	// Determine the target of the message (a user or a channel)
	std::string target = command->getParams()[0];
	std::vector<User *> targetUser;
	// If the target begins with a '#' character, it's a channel
	if (*target.begin() == '#') {
		// Check if the channel exists
		if (command->getServer().isChannel(target)) {
			// Get the channel object
			Channel *chann = command->getServer().getChannel(target);
			// Get the list of members in the channel
			targetUser = chann->getMembers();
			// Remove the sender of the message from the list of recipients
			std::vector<User *>::iterator it = std::find(targetUser.begin(), targetUser.end(), &command->getUser());
			if (it != targetUser.end())
				targetUser.erase(it);
		} else 
			// If the channel does not exist, reply with error code 404 (cannot send to channel)
			return (command->rpl(404, target));
	} else {
		// If the target is not a channel, it's a user
		if (command->getServer().getUserByNickname(target))
			targetUser.push_back(command->getServer().getUserByNickname(target));
		else
			// If the user does not exist, reply with error code 401 (no such nick/channel)
			return command->rpl(401, target);
	}

	// Send the message to all the recipients
	for (std::vector<User *>::iterator it = targetUser.begin(); it != targetUser.end(); ++it)
		command->getUser().sendMessage(*(*it), "PRIVMSG " + target + " :" + command->getContent());
}
