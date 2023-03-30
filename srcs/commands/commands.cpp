/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 22:33:33 by thamon            #+#    #+#             */
/*   Updated: 2023/03/30 18:02:12 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

Commands::Commands(User *user, Server *server, std::string message) : user(user), server(server), message(message)
{
	size_t pos;
	std::string delimiter(":");
	if ((pos = message.find(delimiter)) != std::string::npos)
	{
		std::string tmp = message.substr(0, pos);
		message.erase(0, pos + delimiter.length());
		content = message;
		message = tmp;
	}
	params = split(message, " ");
	prefix = *(params.begin());
	params.erase(params.begin());

	for (size_t i = 0; i < prefix.length(); ++i)
		prefix[i] = std::toupper(prefix[i]);
}

void	Commands::rpl(User &user, unsigned short id, std::string arg1, std::string arg2, std::string arg3, std::string arg4, std::string arg5, std::string arg6, std::string arg7)
{
	// Conversion de l'ID en une chaîne de caractères et remplissage de gauche avec des zéros pour obtenir 3 caractères
	std::stringstream ssid;
	ssid << id;
	std::string sid = ssid.str();
	while (sid.length() < 3)
		sid = "0" + sid;

	// Construction du message de réponse avec le RPL approprié et les arguments fournis
	std::string message = getRPL(id, arg1, arg2, arg3, arg4, arg5, arg6, arg7);

	// Envoi du message de réponse à l'utilisateur
	user.sendMessage(user, sid + " " + message);
}


void	Commands::rpl(unsigned short id, std::string arg1, std::string arg2, std::string arg3, std::string arg4, std::string arg5, std::string arg6, std::string arg7)
{
	rpl(*user, id, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

// Getter
User						&Commands::getUser() { return (*user); }
Server						&Commands::getServer() { return (*server); }
std::string					Commands::getPrefix() { return (prefix); }
std::string					Commands::getContent() { return (content); }
std::vector<std::string>	Commands::getParams() { return (params); }