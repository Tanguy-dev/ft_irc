/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 00:34:54 by thamon            #+#    #+#             */
/*   Updated: 2023/02/04 00:38:10 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "../server/config/getParams.hpp"
#include "../server/server.hpp"
#include <string>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>

class Server;

enum userStatus
{
	PASSWORD,
	REGISTER,
	ONLINE,
	DELETE
};

class User
{
	friend class Server;

	private:
		int sockfd;

		userStatus status;

		// std::string buffer;

		std::string hostname;
		std::string hostaddr;
		std::string nickName;
		std::string username;

	public:
		User(int sockfd, struct sockaddr_in server_address);
		~User();
};

#endif