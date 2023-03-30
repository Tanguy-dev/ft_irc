/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:15:39 by thamon            #+#    #+#             */
/*   Updated: 2023/03/30 17:54:01 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"

void PASS(Commands *command)
{
	if (!command->getParams().size())
		return (command->rpl(461, "PASS"));
	if (command->getUser().getStatus() != PASSWORD)
		return (command->rpl(462));
	if (command->getParams()[0] != command->getServer().getConfig().get("password"))
	{
		command->rpl(464);
		return (command->getUser().setStatus(DELETE));
	}

	if (command->getServer().getConfig().get("password") == command->getParams()[0])
		command->getUser().setStatus(REGISTER);
}