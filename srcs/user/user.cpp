/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 00:34:52 by thamon            #+#    #+#             */
/*   Updated: 2023/02/14 01:49:53 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"
#include "../utils/utils.hpp"

#define BUFFER_SIZE 4096

User::User(int sockfd, struct sockaddr_in const addr)
	: sockfd(sockfd)
{
	// Configure la socket en mode non-bloquant
	fcntl(sockfd, F_SETFL, O_NONBLOCK);

	// Enregistre l'adresse IP du client
	this->host_addr = inet_ntoa(addr.sin_addr);

	// Enregistre le nom d'hôte du client
	char hostname[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *) &addr, sizeof(addr), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		std::cerr << "nul" << std::endl;
	else
		this->hostname = hostname;

	send("welcome on ft_irc !\n\r");

	// Envoyer un message de bienvenue au client
	// char buffer[1024];
	// strcpy(buffer, "Bienvenue sur le serveur IRC!\r\n");
	// send(client_sockfd, buffer, strlen(buffer), 0);

	// Recevoir des données du client
	// memset(buffer, 0, 1024);
	// int n = read(client_sockfd, buffer, 1024 - 1);
	// if (n < 0)
	//	error("ERROR: Ne peut pas lire les données du client.", true);

	// std::cout << "Message reçu du client: " << buffer << std::endl;

	//// Envoyer une réponse au client
	// strcpy(buffer, "Message reçu");
	// std::cout << buffer << std::endl;
	// n = write(client_sockfd, buffer, strlen(buffer));
	// if (n < 0)
	//	error("ERROR: Ne peut pas envoyer de réponse au client.", true);

	//// Fermer la socket du client
	// close(client_sockfd);
	//// Fermer la socket du serveur
	// close(sockfd);
}

User::~User(void)
{
	close(sockfd);
}

void		User::send(std::string const str) const
{
	std::string tmp;

	tmp = "ft_irc: ";
	tmp += str;
	::send(this->sockfd, tmp.c_str(), tmp.length(), 0);
}

void		User::readUserInfo(void)
{
	// Reçoit les données envoyées par le client
	char	buffer[BUFFER_SIZE];
	// ssize_t permets de recevoir un valeur negative, donc recevor une erreur
	ssize_t size = recv(sockfd, &buffer, BUFFER_SIZE, 0);
	 if (size < 0) {
        error("Erreur lors de la réception des données", false);
        return ;
    }

	// Si size == 0, cela signifie que le client s'est déconnecté
	if (size == 0) {
		status = DELETE;
	}
	buffer[size] = 0;
	this->buffer += buffer;
	
	// Sépare les différents messages reçus (séparés par \r\n)
	std::string end("\r\n");
	size_t pos;
	while ((pos = this->buffer.find(end)) != std::string::npos)
	{
		std::string message = this->buffer.substr(0, pos);
		this->buffer.erase(0, pos + end.length());
		if (!message.length())
			continue ;
		// Je vérifie si le message commence par 'NICK' et si oui, j'assigne le nickname à notre variable 'nickname'.
		if (startWith(message, "NICK"))
			nickname = message.substr(5);
		// std::cout << message << " fd : (" << sockfd << ")" << std::endl;
	}
}


std::string User::getUsername(void) const { return (this->username); }
std::string	User::getNickname(void) const { return (this->nickname); }
std::string	User::getHostname(void) const { return (this->hostname); }
userStatus	User::getStatus(void) const { return status; }
int			User::getFd(void) const { return sockfd; }

void		User::setStatus(userStatus status) { this->status = status; }