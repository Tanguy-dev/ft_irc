/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 23:06:09 by thamon            #+#    #+#             */
/*   Updated: 2023/02/06 18:21:32 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "config/getParams.hpp"
#include "../users/user.hpp"
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
	GetParams config;
	std::map<int, User *> users;
	int sockfd;

	std::vector<pollfd> fds;

	void acceptUser();
public:
	Server();
	~Server();

	GetParams &getConfig();

	void init();
	void execute();

	User *getUser(std::string name);
	std::vector<User *> getUsers();
};

#endif