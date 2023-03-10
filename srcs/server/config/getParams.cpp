/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getParams.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 23:30:34 by thamon            #+#    #+#             */
/*   Updated: 2023/02/23 02:47:46 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "getParams.hpp"
#include <iosfwd>
#include "../../utils/utils.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>

void	GetParams::init(std::string config) {
	// Ouvre le fichier de configuration en lecture
	std::ifstream ifs(config.c_str(), std::ifstream::in);
	
	// Vérifie que l'ouverture du fichier s'est bien passée
	if (!ifs.is_open()) {
		error("ifstream", true);
	}

	std::string delimiter = "=";
	std::string line;

	// Lit le fichier ligne par ligne jusqu'à la fin
	while (std::getline(ifs, line)) {
		// Recherche la position du premier "=" dans la ligne
		size_t position = line.find('=');
		
		// Si la ligne ne contient pas de "=", on passe à la ligne suivante
		if (position == std::string::npos) {
			continue;
		}

		// Extrait la clé et la valeur à partir de la ligne lue
		std::string key = line.substr(0, position);
		std::string value = line.substr(position + delimiter.length());

		// Si la valeur commence par "./", cela signifie qu'elle contient le nom d'un fichier à inclure
		if (value.find("./") == 0) {
			// Ouvre le fichier et lit son contenu dans un tampon
			std::ifstream file(value.c_str(), std::ifstream::in);
			std::stringstream buffer;
			buffer << file.rdbuf();

			// Utilise le contenu du tampon comme valeur pour cette clé
			value = buffer.str();
		}

		// Ajoute la paire clé-valeur au dictionnaire
		values[key] = value;
	}

	// Ferme le fichier de configuration
	ifs.close();
}

GetParams::GetParams() { init("config/config.conf"); }

void		GetParams::set(std::string name, std::string value) {
	values[name] = value;
	std::cout << values[name] << std::endl;
}

std::string	GetParams::get(std::string name) {
	return (values[name]);
}