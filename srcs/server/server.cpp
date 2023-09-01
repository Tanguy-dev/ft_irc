/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 23:39:33 by thamon            #+#    #+#             */
/*   Updated: 2023/04/05 17:47:36 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "../user/user.hpp"
#include "../utils/utils.hpp"

#define WHITE "\033[1;37m"
#define YELLOW "\033[0;33m"

Server::Server(void) : upTime(currentTime()), last_ping(std::time(0))
{
	display.define(0, "welcome on ft_irc !");
}

Server::~Server(void)
{
	// Je supprime tous les utilisateurs quand on ferme le serveur.
	std::cout << "Destructor" << std::endl;
	std::vector<User *> users = getUsers();
	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
		delUser(*(*it));
}

std::string Server::getUpTime() { return (upTime); }

DisplayConsole Server::getDisplay() { return (display); }

GetParams &Server::getConfig(void)
{
	return (this->config);
}

std::vector<User *> Server::getUsers()
{
	std::vector<User *> users = std::vector<User *>();

	for (std::map<int, User *>::iterator it = this->users.begin(); it != this->users.end(); ++it)
		users.push_back(it->second);
	return (users);
}

void Server::init()
{
	int re_use = 1;

	// Créer la socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("unable to open the socket", true);

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &re_use, sizeof(re_use)) < 0)
	{
		std::cerr << "unable to set the waiting time for receipt" << std::endl;
		return;
	}

	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
		error("fcntl", true);

	struct sockaddr_in address;
	// Configurer les détails de l'adresse pour la socket
	address.sin_family = AF_INET;
	address.sin_port = htons(atoi(this->config.get("port").c_str()));
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	// Lier la socket à l'adresse
	if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
		error("unable to link the socket", true);

	if (listen(sockfd, 128) < 0)
		error("listen", true);

	fds.push_back(pollfd());
	fds.back().fd = sockfd;
	fds.back().events = POLLIN;

	config.set("userMode", "iO");
	config.set("channelMode", "lki");

	char buffer[128];
	sprintf(buffer, "The server is running on port : %s\nThe password of the server is : %s", this->config.get("port").c_str(), this->config.get("password").c_str());
	display.define(1, buffer);
}

void Server::acceptUser()
{
	char buffer[42];

	struct sockaddr_in address;
	socklen_t client_len = sizeof(address);
	int sockfd = accept(this->sockfd, (struct sockaddr *)&address, &client_len);
	if (sockfd == -1)
		return;

	users[sockfd] = new User(sockfd, address);
	if (!config.get("password").length())
		users[sockfd]->setStatus(REGISTER);

	fds.push_back(pollfd());
	fds.back().fd = sockfd;
	fds.back().events = POLLIN;

	// Affiche le nombre de users connectes
	sprintf(buffer, "\nNumber of users connected : %lu\n", users.size());
	display.define(2, buffer);
}

void Server::execute()
{
	std::vector<User *> users = getUsers();

	if (poll(&fds[0], fds.size(), (60 * 1000) / 10) == -1)
		return;

	display.define(20, "wait");
	// Je vérifie si un nouvel utilisateur cherche à se connecter. Si oui, je le accepte, sinon je vais lire les informations des utilisateurs.
	if (fds[0].revents == POLLIN)
		acceptUser();
	else
	{
		display.define(20, "wait");
		for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); ++it)
		{
			if ((*it).revents == POLLIN)
			{
				this->users[(*it).fd]->readUserInfo(this);
				display.define(20, "Send");
			}
		}
	}

	// Je fais une boucle pour vérifier si aucun utilisateur ne s'est déconnecté.
	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
		if ((*it)->getStatus() == DELETE)
			delUser(*(*it));
	users = getUsers();
	// J'affiche tous les messages qui étaient en attente d'être envoyés.
	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
		(*it)->push();
	displayUsers(users);
}

void Server::delUser(User &user)
{
	std::vector<User *> broadcast_users = std::vector<User *>();
	broadcast_users.push_back(&user);

	std::vector<Channel> remove;
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		if ((*it)->isMember(&user))
		{
			(*it)->quit(&user);

			std::vector<User *> users = (*it)->getMembers();
			if (!users.size())
				remove.push_back(*(*it));
			else
				for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
					if (std::find(broadcast_users.begin(), broadcast_users.end(), *it) == broadcast_users.end())
						broadcast_users.push_back(*it);
		}

	for (std::vector<Channel>::iterator it = remove.begin(); it != remove.end(); ++it)
		delChannel(*it);

	for (std::vector<User *>::iterator it = broadcast_users.begin(); it != broadcast_users.end(); ++it)
		user.sendMessage(*(*it), "QUIT : Client Quit");
	user.push();

	for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); ++it)
	{
		if ((*it).fd == user.getFd())
		{
			fds.erase(it);
			break;
		}
	}

	display.deleteText(user.getFd());

	users.erase(user.getFd());

	delete &user;

	char buffer[42];
	sprintf(buffer, "\nNumber of users connected : %lu/10\n", users.size());
	display.define(2, buffer);
}

void Server::displayUsers(std::vector<User *> users)
{
	char buffer[128];
	// Le sprintf sert a stocker les donner dans un variable plutot que de les ecrire directement dans le terminal
	sprintf(buffer, "%-4s %-11s %-8s", "FD", "Hostname", "Nickname");
	display.define(sockfd, buffer);
	// Je check tous les users connectés et je mets leurs info dans le buffer puis je les envoie pour qu'ils soient écrits.
	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		// Le sprintf sert a stocker les donner dans un variable plutot que de les ecrire directement dans le terminal
		sprintf(buffer, "\033[0;33m%-4i %-11s %-8s", (*it)->getFd(), this->users[(*it)->getFd()]->getHostname().c_str(), this->users[(*it)->getFd()]->getNickname().c_str());
		display.define((*it)->getFd(), buffer);
	}
}

User *Server::getUserByNickname(std::string nickname)
{
	if (nickname.empty())
		throw std::exception();

	std::vector<User *> users = getUsers();
	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
		if (!(*it)->getNickname().empty() && (*it)->getNickname() == nickname)
			return (*it);

	return (NULL);
}

User *Server::getUserByPrefix(std::string prefix)
{
	if (prefix.empty())
		throw std::exception();

	std::vector<User *> users = getUsers();
	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it) {
		if (!(*it)->getPrefix().empty()) {
			std::string userPrefix = (*it)->getPrefix();
			size_t pos = userPrefix.find('!');
			if (pos != std::string::npos) {
				userPrefix.insert(pos + 1, "*");
				userPrefix.replace(0, pos, "*");
			}
			if (userPrefix == prefix)
				return (*it);
		}
	}
	return (NULL);
}

User *Server::getUser(std::string &name)
{
	for (std::map<int, User *>::iterator it = users.begin(); it != users.end(); ++it)
		if ((*it).second->getNickname() == name)
			return ((*it).second);
	return (NULL);
}

bool Server::isChannel(std::string const &name)
{
	bool found = false;
	
	std::cout << name << std::endl;
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->getName() == name)
		{
			found = true;
			break;
		}
	}
	return (found);
}

void Server::delChannel(Channel channel)
{
	char buffer[42];

	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (channel.getName() == (*it)->getName())
		{
			channels.erase(it);
			delete (*it);
			break;
		}
	}
	sprintf(buffer, "\nNumber of channel : %lu\n", channels.size());
	display.define(42, buffer);
}

Channel *Server::getChannel(std::string name)
{
	char buffer[42];
	bool exist = isChannel(name);
	Channel *channel = NULL;

	if (exist)
	{
		for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			if ((*it)->getName() == name)
			{
				channel = *it;
				break;
			}
		}
	}
	else
	{
		channel = new Channel(name);
		channels.push_back(channel);
	}

	sprintf(buffer, "\nNumber of channel : %lu\n", channels.size());
	display.define(42, buffer);

	return (channel);
}

std::vector<Channel *> Server::getChannels()
{
	return (channels);
}