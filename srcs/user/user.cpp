/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 00:34:52 by thamon            #+#    #+#             */
/*   Updated: 2023/03/13 23:18:51 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"
#include "../utils/utils.hpp"

#define BUFFER_SIZE 4096

void PASS(Commands *command);
void NICK(Commands *command);
void USER(Commands *command);
void QUIT(Commands *command);
void PING(Commands *command);
void PONG(Commands *command);

void JOIN(Commands *command);
void PART(Commands *command);

void KICK(Commands *command);
void MODE(Commands *command);
void UNBAN(Commands *command);
void KICKBAN(Commands *command);
void OP(Commands *command);
void DEOP(Commands *command);
void TOPIC(Commands *command);
void INVITE(Commands *command);
void KICKALL(Commands *command);
void BANLIST(Commands *command);
void CLEAR(Commands *command);
void WHOIS(Commands *command);

void PRIVMSG(Commands *command);


void messageConnection(Commands *command)
{
	command->rpl(1, command->getUser().getPrefix());
	command->rpl(2, command->getUser().getHostname(), command->getServer().getConfig().get("version"));
	command->rpl(3, command->getUser().getPrefix());
	command->rpl(4, command->getServer().getConfig().get("name"), command->getServer().getConfig().get("version"));
}

// Cette fonction sert a verifier si la commande est autoriser a s'executer
void User::sort(void)
{
	userStatus last_status = this->status;
	if (last_status == DELETE)
		return;

	std::vector<Commands *> del = std::vector<Commands *>();
	for (std::vector<Commands *>::iterator it = commands.begin(); it != commands.end(); ++it)
	{
		if (last_status == PASSWORD)
		{
			if ((*it)->getPrefix() != "PASS")
				continue;
		}
		else if (last_status == REGISTER)
		{
			if ((*it)->getPrefix() != "NICK" && (*it)->getPrefix() != "USER")
				continue;
		}

		// Si la commands a un préfixe valide, il appelle la fonction correspondante avec la commande comme argument.
		if (command_function.count((*it)->getPrefix()))
			command_function[(*it)->getPrefix()]((*it));
		// Ajoute la commande au vecteur del
		del.push_back((*it));
	}

	// Supprime tous les éléments de commands qui ont été stockés dans del.
	for (std::vector<Commands *>::iterator it = del.begin(); it != del.end(); ++it)
	{
		if (std::find(commands.begin(), commands.end(), *it) != commands.end())
		{
			commands.erase(std::find(commands.begin(), commands.end(), *it));
			delete (*it);
		}
	}

	if (last_status == REGISTER)
		if (nickname.length() && realname.length())
			status = ONLINE;

	if (last_status != status)
	{
		if (status == ONLINE)
			messageConnection(*commands.begin());
		sort();
	}
}

User::User(int sockfd, struct sockaddr_in addr) : command_function(), sockfd(sockfd), last_ping(std::time(0)), buffer(), status(PASSWORD), nickname(), username(), realname(), host_addr(), hostname()
{
	// Configure la socket en mode non-bloquant
	fcntl(sockfd, F_SETFL, O_NONBLOCK);

	// Enregistre l'adresse IP du client
	host_addr = inet_ntoa(addr.sin_addr);

	// Enregistre le nom d'hôte du client
	char hostname[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *)&addr, sizeof(addr), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		error("getnameinfo", false);
	else
		this->hostname = hostname;

	command_function["PASS"] = PASS;
	command_function["NICK"] = NICK;
	command_function["USER"] = USER;
	command_function["QUIT"] = QUIT;

	command_function["JOIN"] = JOIN;
	command_function["PART"] = PART;
	command_function["PING"] = PING;
	command_function["PONG"] = PONG;
	command_function["PRIVMSG"] = PRIVMSG;
	
	// command_function["KICK"] = KICK;
	command_function["BAN"] = MODE;
	// command_function["UNBAN"] = UNBAN;
	// command_function["KICKBAN"] = KICKBAN;
	// command_function["OP"] = OP;
	// command_function["DEOP"] = DEOP;
	// command_function["TOPIC"] = TOPIC;
	// command_function["INVITE"] = INVITE;
	// command_function["KICKALL"] = KICKALL;
	// command_function["BANLIST"] = BANLIST;
	// command_function["CLEAR"] = CLEAR;
	// command_function["WHOIS"] = WHOIS;
}

User::~User()
{
	close(sockfd);
}

// void		User::send(std::string const str) const
// {
// 	std::string tmp;

// 	tmp = "ft_irc: ";
// 	tmp += str;
// 	::send(this->sockfd, tmp.c_str(), tmp.length(), 0);
// }

void User::sendMessage(User &toUser, std::string message)
{
	toUser.write(":" + this->getPrefix() + " " + message);
}

void User::write(std::string message)
{
	waitMessage.push_back(message);
}

void User::push()
{
	// Check si il y a des message en attente d'envoi
	if (!waitMessage.size())
		return;

	std::string buffer;
	for (std::vector<std::string>::iterator it = waitMessage.begin(); it != waitMessage.end(); ++it)
		buffer += *it + "\r\n";
	waitMessage.clear();

	// envoie les messages qui etais en attentes
	if (buffer.length())
		if (send(sockfd, buffer.c_str(), buffer.length(), 0) == -1)
			error("send", false);
}

void User::readUserInfo(Server *server)
{
	{
		// Reçoit les données envoyées par le client
		char buffer[BUFFER_SIZE + 1];
		// ssize_t permets de recevoir un valeur negative, donc recevor une erreur
		ssize_t size;
		if ((size = recv(sockfd, &buffer, BUFFER_SIZE, 0)) == -1)
			return;

		// Si size == 0, cela signifie que le client s'est déconnecté
		if (size == 0)
		{
			status = DELETE;
			return;
		}

		buffer[size] = 0;
		this->buffer += buffer;
	}

	// Sépare les différents messages reçus (séparés par \r\n)
	std::string end("\r\n");
	size_t pos;
	while ((pos = buffer.find(end)) != std::string::npos)
	{
		std::string message = buffer.substr(0, pos);

		buffer.erase(0, pos + end.length());
		if (!message.length())
			continue;
		commands.push_back(new Commands(this, server, message));
	}
	sort();
}

std::string User::getPrefix(void)
{
	if (status == PASSWORD || status == REGISTER)
		return (std::string(""));

	std::string prefix = nickname;
	if (getHost().length())
	{
		if (username.length())
			prefix += "!" + username;
		prefix += "@" + getHost();
	}
	return (prefix);
}

std::string User::getUsername(void) { return (this->username); }
std::string User::getRealname(void) { return (this->realname); }
std::string User::getNickname(void) { return (this->nickname); }
std::string User::getHostname(void) { return (this->hostname); }
userStatus User::getStatus(void) { return (status); }
time_t User::getLastPing(void) { return (last_ping); };
int User::getFd(void) { return (sockfd); }
std::string User::getHost(void)
{
	if (hostname.size())
		return (hostname);
	return (host_addr);
}

void User::setStatus(userStatus status) { this->status = status; }
void User::setNickname(std::string nickname) { this->nickname = nickname; }
void User::setUsername(std::string username) { this->username = username; }
void User::setRealname(std::string realname) { this->realname = realname; }
void User::setLastPing(time_t last_ping) { this->last_ping = last_ping; }