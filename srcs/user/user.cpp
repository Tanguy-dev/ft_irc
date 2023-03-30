/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 00:34:52 by thamon            #+#    #+#             */
/*   Updated: 2023/03/30 19:26:37 by thamon           ###   ########.fr       */
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
void INVITE(Commands *command);
void TOPIC(Commands *command);
void NAMES(Commands *command);

void PRIVMSG(Commands *command);
void NOTICE(Commands *command);
void WHO(Commands *command);

void MODE(Commands *command);
void KICK(Commands *command);
void OPER(Commands *command);
void KILL(Commands *command);



void messageConnection(Commands *command)
{
	command->rpl(1, command->getUser().getPrefix());
	command->rpl(2, command->getUser().getHostname(), command->getServer().getConfig().get("version"));
	command->rpl(3, command->getServer().getUpTime());
	command->rpl(4, command->getServer().getConfig().get("name"), command->getServer().getConfig().get("version"), command->getServer().getConfig().get("userMode"), command->getServer().getConfig().get("channelMode"));
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
		del.push_back(*it);
	}

	// Supprime tous les éléments de commands qui ont été stockés dans del.
	for (std::vector<Commands *>::iterator it = del.begin(); it != del.end(); it++)
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

User::User(int sockfd, struct sockaddr_in addr) : command_function(), sockfd(sockfd), last_ping(std::time(0)), buffer(), status(PASSWORD), nickname(), username(), realname(), host_addr(), hostname(), mode("i")
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
	command_function["INVITE"] = INVITE;
	command_function["TOPIC"] = TOPIC;
	command_function["NAMES"] = NAMES;
	
	command_function["PING"] = PING;
	command_function["PONG"] = PONG;
	
	command_function["PRIVMSG"] = PRIVMSG;
	command_function["NOTICE"] = NOTICE;
	command_function["WHO"] = WHO;
	
	command_function["MODE"] = MODE;
	command_function["KICK"] = KICK;
	command_function["OPER"] = OPER;
	command_function["KILL"] = KILL;
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
std::string User::getChannel(void) { return (this->channel); }
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
void User::setChannel(std::string channel) { this->channel = channel; }

void User::setMode(char mode, bool active)
{
    if (active) {
        // Ajoute le mode si il n'est pas déjà présent
        if (this->mode.find(mode) == std::string::npos) {
            this->mode += mode;
        }
    } else {
        // Supprime le mode s'il est présent
        size_t index = this->mode.find(mode);
        if (index != std::string::npos) {
            this->mode.erase(index, 1);
        }
    }
}

std::string User::getMode()
{
	return (mode);
}

bool User::isModeActived(char mode)
{
	return (this->mode.find(mode) != std::string::npos);
}

bool User::isAdmin() {
    return (isModeActived('O'));
}