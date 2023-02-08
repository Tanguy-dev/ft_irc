/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 20:41:38 by gadeneux          #+#    #+#             */
/*   Updated: 2023/02/06 16:07:35 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

const int BUFSIZE = 1024;

int main(int argc, char *argv[])
{
    // Définir les variables pour la socket
    int sockfd, client_sockfd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_len;

    // Créer la socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        cerr << "ERROR: Ne peut pas ouvrir la socket." << endl;
        return -1;
    }

    // Configurer les détails de l'adresse pour la socket
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(6667);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Lier la socket à l'adresse
    if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        cerr << "ERROR: Ne peut pas lier la socket." << endl;
        return -1;
    }

    // Écouter pour les nouvelles connexions
    listen(sockfd, 5);
    cout << "Serveur en attente de connexions..." << endl;

    client_len = sizeof(client_address);
    while (true)
    {
        // Accepter une nouvelle connexion
        client_sockfd = accept(sockfd, (struct sockaddr *)&client_address, &client_len);
        if (client_sockfd < 0)
        {
            cerr << "ERROR: Ne peut pas accepter la connexion." << endl;
            return -1;
        }

        cout << "Nouvelle connexion établie avec un client." << endl;

        // Envoyer un message de bienvenue au client
        char buffer[BUFSIZE];
        strcpy(buffer, "Bienvenue sur le serveur IRC!\r\n");
        send(client_sockfd, buffer, strlen(buffer), 0);

        // Recevoir des données du client
        memset(buffer, 0, BUFSIZE);
        int n = read(client_sockfd, buffer, BUFSIZE - 1);
        if (n < 0)
        {
            cerr << "ERROR: Ne peut pas lire les données du client." << endl;
            return -1;
        }
        cout << "Message reçu du client: " << buffer << endl;

        // Envoyer une réponse au client
        strcpy(buffer, "Message reçu");
        std::cout << buffer << std::endl;
        n = write(client_sockfd, buffer, strlen(buffer));
        if (n < 0)
        {
            cerr << "ERROR: Ne peut pas envoyer de réponse au client." << endl;
            return -1;
        }

        // Fermer la socket du client
        // close(client_sockfd);
    }

    // Fermer la socket du serveur
    close(sockfd);

    return 0;
}