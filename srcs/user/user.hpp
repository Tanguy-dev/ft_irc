/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 00:34:54 by thamon            #+#    #+#             */
/*   Updated: 2023/03/24 00:44:12 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "../server/config/getParams.hpp"
#include "../server/server.hpp"
#include "../commands/commands.hpp"
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
#include <algorithm>
#include <ostream>

class Server;
class Channel;
class Commands;

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
		std::map<std::string, void (*)(Commands *)>	command_function;
		std::vector<Commands *>	commands;

		int					sockfd;


		time_t				last_ping;
		std::string			buffer;
		userStatus			status;
	
		std::string			nickname;
		std::string			username;
		std::string			realname;
		
		std::string			host_addr;
		std::string			hostname;
		std::string 		channel;

		std::string 		mode;

		std::vector<std::string>	waitMessage;

		void				sort(void);
		void 				write(std::string message);
	public:
	
		User(int sockfd, struct sockaddr_in addr);
		~User();


		// void		send(std::string const str) const;
		void		readUserInfo(Server *server);

		// getters
		std::string	getUsername(void);
		std::string	getNickname(void);
		std::string getRealname(void);
		std::string	getHostname(void);
		std::string getPrefix(void);
		std::string getHost(void);
		std::string getChannel(void);

		time_t		getLastPing(void);
		userStatus	getStatus(void);
		int			getFd(void);

		// setters
		void		setStatus(userStatus status);
		void		setNickname(std::string nickname);
		void		setUsername(std::string username);
		void		setRealname(std::string realname);
		void		setLastPing(time_t last_ping);
		void		setChannel(std::string channel);

		// Gestion des message vers le client
		void 		sendMessage(User &toUser, std::string message);
		void 		push();

		// Mode utilisateur
		void 			setMode(char mode, bool active);
		bool 			isModeActived(char mode);
		std::string 	getMode();

		bool isAdmin();
};

#endif