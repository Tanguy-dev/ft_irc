/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gadeneux <gadeneux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 17:11:12 by gadeneux          #+#    #+#             */
/*   Updated: 2023/02/10 18:38:52 by gadeneux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#ifndef LOGGER_HPP
# define LOGGER_HPP

class Logger
{
	private:
	
		bool		print_info;
	
	public:

		Logger(void);
		~Logger(void);

		template<typename T>
		void		log(T str)
		{
			if (this->print_info)
				std::cout << "[INFO] ";
				
			std::cout << str;
			this->print_info = false;
		}

		template<typename T>
		void		logln(T str)
		{
			if (this->print_info)
				std::cout << "[INFO] ";
				
			std::cout << str << std::endl;
			this->print_info = true;
		}
};

#endif