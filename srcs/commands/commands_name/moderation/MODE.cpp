/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 14:21:05 by gadeneux          #+#    #+#             */
/*   Updated: 2023/03/13 22:22:59 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../commands.hpp"

void MODE(Commands *command)
{
	Channel *chann = command->getServer().getChannel(command->getParams()[0]);

	if (!chann)
	{
		// Le channel n'existe pas
		return (command->rpl(403));
	}
	
	if (!chann->isMember(&(command->getUser())))
	{
		// L'utilisateur n'est pas membre du channel
		return (command->rpl(442));
	}

	std::string type = command->getParams()[1];

	if (type == "+b")
	{
		if (!chann->isOperator(&command->getUser()))
		{
			// L'utilisateur n'a pas les privilèges pour bannir
			return (command->rpl(482));
		}
		
		if (command->getParams().size() == 4)
		{
			User * user = command->getServer().getUserByPrefix(command->getParams()[2]);
			int delay_in_seconds = atoi(command->getParams()[3].c_str());
		
			if (user == NULL)
			{
				// L'utilisateur n'est pas présent sur le serveur
				return (command->rpl(401));
			}

			if (chann->ban(user, delay_in_seconds))
				/* Send feedback message from server to client */;
		}
		
	} else if (type == "-b")
	{
		if (!chann->isOperator(&command->getUser()))
		{
			// L'utilisateur n'a pas les privilèges pour dé-bannir
			return (command->rpl(482));
		}
			
		if (command->getParams().size() == 3)
		{
			User * user = command->getServer().getUserByPrefix(command->getParams()[2]);

			if (user == NULL)
			{
				// L'utilisateur n'est pas présent sur le serveur
				return (command->rpl(401));
			}

			if (chann->unban(user))
				/* Send feedback message from server to client */;
		}
	} else if (type == "+o")
	{
		if (!chann->isOperator(&command->getUser()))
		{
			// L'utilisateur n'a pas les privilèges pour promouvoir opérateur
			return (command->rpl(482));
		}
			
		if (command->getParams().size() == 3)
		{
			User * user = command->getServer().getUserByPrefix(command->getParams()[2]);

			if (user == NULL)
			{
				// L'utilisateur n'est pas présent sur le serveur
				return (command->rpl(401));
			}

			if (chann->op(user))
				/* Send feedback message from server to client */;
		}
	} else if (type == "-o")
	{
		if (!chann->isOperator(&command->getUser()))
		{
			// L'utilisateur n'a pas les privilèges démouvoir opérateur
			return (command->rpl(482));
		}
			
		if (command->getParams().size() == 3)
		{
			User * user = command->getServer().getUserByPrefix(command->getParams()[2]);
			
			if (user == NULL)
			{
				// L'utilisateur n'est pas présent sur le serveur
				return (command->rpl(401));
			}

			if (chann->deop(user))
				/* Send feedback message from server to client */;
		}
	}
}
