/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 23:06:09 by thamon            #+#    #+#             */
/*   Updated: 2023/02/14 00:52:26 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "config/getParams.hpp"
#include "config/displayConsole.hpp"
#include "../logger/logger.hpp"
#include "../user/user.hpp"
#include <string>
#include <fcntl.h>
#include <csignal>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>
#include <poll.h>
#include <map>
#include <vector>

class User;

class Server
{
	private:

		GetParams				config;
		Logger					logger;
		displayConsole			display;
		int						sockfd;

		std::map<int, User *>	users;
		std::vector<pollfd>		fds;

		int 					incrementation;
	
	private:

		void					acceptUser(void);
		void					delUser(User &user);
		void					displayUsers(std::vector<User *> users);
		
	public:
	
		Server(void);
		~Server(void);

		GetParams				&getConfig(void);
		Logger					&getLogger(void);

		void					init(void);
		void					execute(void);

		User *getUser(std::string name);
		std::vector<User *> getUsers();
};

#endif