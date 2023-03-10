/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 02:46:15 by thamon            #+#    #+#             */
/*   Updated: 2023/03/07 03:18:37 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"

void QUIT(Commands *command)
{
	if (command->getContent().length() > 1)
		command->getUser().sendMessage(command->getUser(), "QUIT :" + command->getContent());
	command->getUser().setStatus(DELETE);
}