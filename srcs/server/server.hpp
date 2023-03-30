/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 23:06:09 by thamon            #+#    #+#             */
/*   Updated: 2023/03/27 17:16:17 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "config/getParams.hpp"
#include "config/displayConsole.hpp"
#include "../user/user.hpp"
#include "../channel/channel.hpp"
#include <string>
#include <fcntl.h>
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
#include <ctime>

class User;

class Server
{
	private:

		GetParams				config;
		DisplayConsole			display;
		int						sockfd;

		std::vector<Channel *>	channels;
		std::map<int, User *>	users;
		std::vector<pollfd>		fds;

		std::string upTime;
		time_t last_ping;
	
	private:

		void					acceptUser(void);
		void					delUser(User &user);
		void					displayUsers(std::vector<User *> users);
		
	public:
	
		Server(void);
		~Server(void);

		GetParams				&getConfig(void);
		User					*getUserByNickname(std::string nickname);
		User					*getUserByPrefix(std::string nickname);
		Channel					*getChannel(std::string name);
		std::string				getUpTime();
		
		void					init(void);
		void					execute(void);

		User *getUser(std::string &name);
		std::vector<User *> getUsers();
		std::vector<Channel *> getChannels();
		DisplayConsole 		getDisplay();

		bool isChannel(std::string const &name);
		void delChannel(Channel channel);
};

#endif