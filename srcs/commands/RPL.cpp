/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 01:04:54 by thamon            #+#    #+#             */
/*   Updated: 2023/03/10 02:57:06 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "commands.hpp"

std::string ERR_NOORIGIN() { return (":No origin specified"); }

std::string ERR_NORECIPIENT() { return (":No recipient"); }
std::string ERR_NOTEXTTOSEND() { return (":No text to send"); }

std::string ERR_NOSUCHNICK(std::string nick) { return (nick + ":No such nick/channel"); }

// Channels
std::string RPL_NAMREPLY(std::string symbole, std::string channel, std::string nick) { return (symbole + " " + channel + " :" + nick); }
std::string RPL_ENDOFNAMES(std::string channel) { return (channel + " :End of /NAMES list"); }
std::string ERR_CANNOTSENDTOCHAN(std::string channel) { return (channel + " :Cannot send to channel"); }

// Error nickname
std::string ERR_NONICKNAMEGIVEN() { return (":No nickname given"); }
std::string ERR_ERRONEUSNICKNAME(std::string nick) { return (nick + " :Erroneus nickname"); }
std::string ERR_NICKNAMEINUSE(std::string nick) { return (nick + " :Nickname is already in use"); }
std::string ERR_NICKCOLLISION(std::string nick) { return (nick + " :Nickname collision KILL"); }

// Error password
std::string ERR_NEEDMOREPARAMS(std::string cmd) { return (cmd + " :Not enough parameters"); }
std::string ERR_ALREADYREGISTERED() { return (":You may not reregister"); }
std::string ERR_PASSWDMISMATCH() { return (":Password incorrect"); }


// Message de connection
std::string RPL_WELCOME(std::string prefix) { return (":Welcome to the IRC Network, " + prefix); }
std::string RPL_YOURHOST(std::string servername, std::string ver) { return (":Your host is " + servername + ", running version " + ver); }
std::string RPL_CREATED(std::string date) { return (":This server was created " + date); }
std::string RPL_MYINFO(std::string servername, std::string ver) { return (servername + " " + ver + "<available user modes>\n<available channel modes> [<channel modes with a parameter>]"); }

std::string Commands::getRPL(unsigned short id, std::string arg1, std::string arg2, std::string arg3, std::string arg4)
{
	std::string target;

	if (user->getStatus() == PASSWORD || user->getStatus() == REGISTER)
		target = "*";
	else
		target = user->getNickname();
	target += " ";

	switch (id)
	{
	case 001:
		return (target + RPL_WELCOME(arg1));
	case 002:
		return (target + RPL_YOURHOST(arg1, arg2));
	case 003:
		return (target + RPL_CREATED(arg1));
	case 004:
		return (target + RPL_MYINFO(arg1, arg2));
	case 353:
		return (target + RPL_NAMREPLY(arg1, arg2, arg3));
	case 366:
		return (target + RPL_ENDOFNAMES(arg1));
	case 401:
		return (target + ERR_NOSUCHNICK(arg1));
	case 404:
		return (target + ERR_CANNOTSENDTOCHAN(arg1));
	case 409:
		return (target + ERR_NOORIGIN());
	case 411:
		return (target + ERR_NORECIPIENT());
	case 412:
		return (target + ERR_NOTEXTTOSEND());
	case 431:
		return (target + ERR_NONICKNAMEGIVEN());
	case 432:
		return (target + ERR_ERRONEUSNICKNAME(arg1));
	case 433:
		return (target + ERR_NICKNAMEINUSE(arg1));
	case 436:
		return (target + ERR_NICKCOLLISION(arg1));
	case 461:
		return (target + ERR_NEEDMOREPARAMS(arg1));
	case 462:
		return (target + ERR_ALREADYREGISTERED());
	case 464:
		return (target + ERR_PASSWDMISMATCH());
	case 000:
		return (target + arg1 + arg2 + arg3 + arg4);
	case 999:
		return ("Test 1234");
	default:
		return (std::string());
	}
}