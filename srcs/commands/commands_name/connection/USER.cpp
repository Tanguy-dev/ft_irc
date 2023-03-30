/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 01:41:23 by thamon            #+#    #+#             */
/*   Updated: 2023/03/30 19:03:42 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"

void USER(Commands *command)
{
	if (command->getParams().size() < 3)
		return (command->rpl(461, "USER"));
	if (command->getUser().getStatus() != REGISTER)
		return (command->rpl(462));

	command->getUser().setUsername(command->getParams()[0]);
	command->getUser().setRealname(command->getContent());
}