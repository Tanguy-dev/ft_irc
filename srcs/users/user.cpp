/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 00:34:52 by thamon            #+#    #+#             */
/*   Updated: 2023/02/06 18:32:52 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"
#include "../utils/utils.hpp"

User::User(int sockfd, struct sockaddr_in server_address) : sockfd(sockfd), status(PASSWORD)
{
	fcntl(sockfd, F_SETFL, O_NONBLOCK);

	// Recevoir des données du client
	char buffer[1024];
	memset(buffer, 0, 1024);
	int n = read(sockfd, buffer, 1024 - 1);
	if (n < 0)
		error("ERROR: Ne peut pas lire les données du client.", true);

	hostaddr = inet_ntoa(server_address.sin_addr);
	char hostname[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *)&server_address, sizeof(server_address), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		std::cerr << "nul" << std::endl;
	else
		this->hostname = hostname;
}
User::~User() { close(sockfd); }