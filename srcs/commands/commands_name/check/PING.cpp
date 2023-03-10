/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 02:30:11 by thamon            #+#    #+#             */
/*   Updated: 2023/02/23 02:59:48 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"

void PING(Commands *command)
{
	if (command->getParams().size() == 0)
		return (command->rpl(409));
	command->getUser().sendMessage(command->getUser(), "PONG :" + command->getParams()[0] + " " + command->getUser().getNickname());
}