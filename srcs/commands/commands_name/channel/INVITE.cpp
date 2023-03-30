/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 23:17:51 by thamon            #+#    #+#             */
/*   Updated: 2023/03/27 17:05:35 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"

void INVITE(Commands *command)
{
	if (command->getParams().size() < 2)
		return (command->rpl(461, "INVITE"));

	if (command->getServer().getUser(command->getParams()[0]) == NULL)
		return (command->rpl(401, command->getParams()[0]));

	if (!command->getServer().isChannel(command->getParams()[1]))
		return (command->rpl(403, command->getParams()[1]));

	if (command->getServer().getChannel(command->getParams()[1])->isMemberName(command->getParams()[0]))
		return (command->rpl(443, command->getParams()[0], command->getParams()[1]));

	if (command->getServer().isChannel(command->getParams()[1]))
	{
		Channel *channel = command->getServer().getChannel(command->getParams()[1]);

		if (channel->getMode().find('i') != std::string::npos && channel->getUserMode(command->getUser()).find('o') == std::string::npos && !command->getUser().isAdmin())
			return (command->rpl(482, command->getParams()[1]));
		else if (!channel->isMemberName(command->getUser().getNickname()))
			return (command->rpl(442, command->getParams()[1]));

		channel->addInvited(command->getServer().getUser(command->getParams()[0]));
	}
	command->getUser().sendMessage(*command->getServer().getUser(command->getParams()[0]), "INVITE " + command->getParams()[0] + " " + command->getParams()[1]);
	command->rpl(341, command->getParams()[0], command->getParams()[1]);
}