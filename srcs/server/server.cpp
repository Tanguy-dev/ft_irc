/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 23:39:33 by thamon            #+#    #+#             */
/*   Updated: 2023/02/06 18:35:40 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "../utils/utils.hpp"

Server::Server()
{
	return;
}

Server::~Server()
{
	return;
}

GetParams &Server::getConfig()
{
	return (this->config);
}

void Server::init()
{
	// Créer la socket
	int reuse = 1;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("Ne peut pas ouvrir la socket.", true);
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
	{
		std::cerr << "Impossible de définir le temps d'attente pour la réception" << std::endl;
		return ;
	}
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
		error("fcntl", true);

	struct sockaddr_in server_address;
	// Configurer les détails de l'adresse pour la socket
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(this->config.get("port").c_str()));
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	// Lier la socket à l'adresse
	if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
		error("Ne peut pas lier la socket.", true);
	if (listen(sockfd, server_address.sin_port) < 0)
		error("listen", true);
	
	fds.push_back(pollfd());
	fds.back().fd = sockfd;
	fds.back().events = POLLIN;

	std::cout << "Serveur en attente de connexions..." << std::endl;
	std::cout << "Le serveur tourne sur le port : " << this->config.get("port") << std::endl;
}

// https://docs.google.com/document/d/1lwBfoSsucgxOUjKfAzbzj1yqT3vBhaC6yKWttitH0Dk/edit?usp=sharing
// Lien documentation
//

void display(std::vector<User *> v)
{
	for (size_t i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << "test\n ";
	}
	std::cout << "\npoulet" << std::endl;
}

void Server::acceptUser() {
	size_t maxUsers = 10;

	if (users.size() == maxUsers)
		if (shutdown(sockfd, SHUT_RD) == -1)
			return;

	struct sockaddr_in client_address;
	socklen_t client_len = sizeof(client_address);
	int client_sockfd;

	// Accepter une nouvelle connexion
	client_sockfd = accept(sockfd, (struct sockaddr *)&client_address, &client_len);
	if (client_sockfd < 0)
			error("ERROR: Ne peut pas accepter la connexion.", true);

	// Envoyer un message de bienvenue au client
	char buffer[1024];
	strcpy(buffer, "Bienvenue sur le serveur IRC!\r\n");
	send(client_sockfd, buffer, strlen(buffer), 0);
	
	// Ajout du nouvel user
	users[client_sockfd] = new User(client_sockfd, client_address);

	fds.push_back(pollfd());
	fds.back().fd = client_sockfd;
	fds.back().events = POLLIN;
	// while (true)
	// {
	// 	std::cout << "Nouvelle connexion établie avec un client." << std::endl;

	// 	// Envoyer un message de bienvenue au client
	// 	char buffer[1024];
	// 	strcpy(buffer, "Bienvenue sur le serveur IRC!\r\n");
	// 	send(client_sockfd, buffer, strlen(buffer), 0);

	// 	// Recevoir des données du client
	// 	memset(buffer, 0, 1024);
	// 	int n = read(client_sockfd, buffer, 1024 - 1);
	// 	if (n < 0)
	// 		error("ERROR: Ne peut pas lire les données du client.", true);

	// 	std::cout << "Message reçu du client: " << buffer << std::endl;

	// 	// Envoyer une réponse au client
	// 	strcpy(buffer, "Message reçu");
	// 	std::cout << buffer << std::endl;
	// 	n = write(client_sockfd, buffer, strlen(buffer));
	// 	if (n < 0)
	// 		error("ERROR: Ne peut pas envoyer de réponse au client.", true);

	// 	// Fermer la socket du client
	// 	close(client_sockfd);
	// }
}

void Server::execute()
{
	std::vector<User *> users = getUsers();
	// display(users);

	if (poll(&fds[0], fds.size(), 10000) == -1)
		return ;
	if (fds[0].revents == POLLIN)
		acceptUser();


	
	// // Fermer la socket du serveur
	// close(sockfd);
}

std::vector<User *> Server::getUsers()
{
	std::vector<User *> users = std::vector<User *>();

	for (std::map<int, User *>::iterator it = this->users.begin(); it != this->users.end(); ++it)
		users.push_back(it->second);
	return (users);
}