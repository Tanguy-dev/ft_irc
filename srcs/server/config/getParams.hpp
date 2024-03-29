/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getParams.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 23:30:32 by thamon            #+#    #+#             */
/*   Updated: 2023/02/22 23:36:22 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GETPARAMS_HPP
#define GETPARAMS_HPP

#include <string>
#include <map>
#include <string>
#include <iostream>

class GetParams
{
	private:
		std::map<std::string, std::string> values;
		void	init(std::string config);
	public:
		GetParams();

		void		set(std::string name, std::string value);
		std::string	get(std::string name);
};

#endif