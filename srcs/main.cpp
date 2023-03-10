/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 23:39:29 by thamon            #+#    #+#             */
/*   Updated: 2023/02/16 03:55:41 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/server.hpp"
#include <csignal>

bool end = false;

void signal_handler(int) { end = true; };

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "./ircserv <port> <password>\n";
		return (0);
	}

	Server server = Server();
	signal(SIGINT, signal_handler);

	server.getConfig().set("port", av[1]);
	server.getConfig().set("password", av[2]);

	server.init();
	while (!end)
		server.execute();
	return (0);
}