/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 00:34:54 by thamon            #+#    #+#             */
/*   Updated: 2023/02/14 01:46:37 by thamon           ###   ########.fr       */
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

		userStatus			status;

		std::string			buffer;
	
		std::string			nickname;
		std::string			username;
		int					sockfd;
		
		std::string			host_addr;
		std::string			hostname;
		
	public:
	
		User(int sockfd, struct sockaddr_in const addr);
		~User(void);


		void		send(std::string const str) const;
		void		readUserInfo(void);

		// getters
		std::string	getUsername(void) const;
		std::string	getNickname(void) const;
		std::string	getHostname(void) const;
		userStatus	getStatus(void) const;
		int			getFd(void) const;

		// setters
		void		setStatus(userStatus status) { this->status = status; }
};

#endif