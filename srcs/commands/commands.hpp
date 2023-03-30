/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 22:33:35 by thamon            #+#    #+#             */
/*   Updated: 2023/03/30 18:02:13 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <vector>
#include "../server/server.hpp"
#include "../utils/utils.hpp"
#include "../user/user.hpp"

class User;
class Server;

class Commands
{
private:
	User						*user;
	Server						*server;

	std::string					prefix;
	std::string 				message;
	std::string					content;

	std::vector<std::string>	params;
	
	std::string					getRPL(unsigned short id, std::string arg1 = "", std::string arg2 = "", std::string arg3 = "", std::string arg4 = "", std::string arg5 = "", std::string arg6 = "", std::string arg7 = "");
public:
	Commands(User *user, Server *server, std::string message);

	void						rpl(User &user, unsigned short id, std::string arg1 = "", std::string arg2 = "", std::string arg3 = "", std::string arg4 = "", std::string arg5 = "", std::string arg6 = "", std::string arg7 = "");
	void						rpl(unsigned short id, std::string arg1 = "", std::string arg2 = "", std::string arg3 = "", std::string arg4 = "", std::string arg5 = "", std::string arg6 = "", std::string arg7 = "");

	User						&getUser();
	Server						&getServer();
	std::string					getPrefix();
	std::string					getContent();
	std::vector<std::string>	getParams();
};


#endif