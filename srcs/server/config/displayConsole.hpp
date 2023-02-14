/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   displayConsole.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 22:55:08 by thamon            #+#    #+#             */
/*   Updated: 2023/02/13 23:00:44 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPLAYCONSOLE_HPP
#define DISPLAYCONSOLE_HPP

#include <iostream>
#include <map>

class displayConsole
{
	
	private:
		std::map<unsigned char, std::string>	text;
		void	display();
		void	clear();
		
	public:
	
		displayConsole();

		void	define(unsigned char i, std::string text);
		void	deleteText(unsigned char i);

};

#endif