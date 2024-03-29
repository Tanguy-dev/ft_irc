/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   displayConsole.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 22:55:10 by thamon            #+#    #+#             */
/*   Updated: 2023/04/05 13:34:41 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "displayConsole.hpp"

void	DisplayConsole::clear()
{
	/** Efface tous le terminal  avec ""\033[2J" et flush permert vider le 
		buffer et de forcer une sortie immediate du buffer*/
	std::cout << "\033[2J" << std::flush;
}

void	DisplayConsole::display()
{
	// Clear le terminal
	clear();
	// une boucle pour write tous les element de la map de text. "\033[0m" reload les couleur et effet (gras, etc...)
	for (std::map<unsigned char, std::string>::iterator it = text.begin(); it != text.end(); ++it)
		std::cout << it->second << "\033[0m" << std::endl;
}

DisplayConsole::DisplayConsole(void) { display(); }

void	DisplayConsole::define(unsigned char i, std::string text)
{
	if (this->text[i] == text)
		return ;
	this->text[i] = text;
	display();
}

void	DisplayConsole::deleteText(unsigned char i)
{
	text.erase(i);
	display();
}
