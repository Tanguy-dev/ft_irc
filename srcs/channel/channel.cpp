/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 21:45:22 by gadeneux          #+#    #+#             */
/*   Updated: 2023/03/14 01:29:30 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"
#include "../user/user.hpp"

Channel::Channel(std::string name) : name(name), topic("") {}

void Channel::join(User *user)
{
	if (isMember(user))
		throw std::exception();

	members[user->getFd()] = user;
}

void Channel::quit(User *user)
{
	if (!isMember(user))
		throw std::exception();

	members.erase(members.find(user->getFd()));
	deop(user);
}

bool Channel::ban(User *user, int delay_in_seconds)
{
	if (isUserBanned(user))
		return (false);

	std::time_t now = std::time(NULL);

	banned_users[user->getPrefix()] = std::make_pair(now, delay_in_seconds);
	return (true);
}

bool Channel::unban(User *user)
{
	if (!isUserBanned(user))
		return (false);

	std::map<std::string, std::pair<std::time_t, int> >::iterator it = banned_users.find(user->getPrefix());
	if (it != banned_users.end())
		banned_users.erase(it);
	return (true);
}

bool Channel::op(User *user)
{
	if (isOperator(user))
		return (false);

	operators.push_back(user);
	return (true);
}

bool Channel::deop(User *user)
{
	if (!isOperator(user))
		return (false);

	operators.erase(std::find(operators.begin(), operators.end(), user));
	return (true);
}

void Channel::setTopic(std::string const &topic)
{
	this->topic = topic;
}

void Channel::sendMessage(User *sender, std::string message)
{
	for (std::map<int, User *>::iterator it = members.begin(); it != members.end(); ++it)
	{
		sender->sendMessage(*(*it).second, message);
	}
}

bool Channel::isMember(User *user) const
{
	// for (size_t i = 0; i < members.size(); i++)
	// 	if (members[i] == user)
	// 		return (true);
	// return (false);
	return (members.find(user->getFd()) != members.end());
}

bool Channel::isUserBanned(User *user)
{
	std::map<std::string, std::pair<std::time_t, int> >::iterator it = banned_users.find(user->getPrefix());
	if (it != banned_users.end())
	{
		std::time_t now = std::time(NULL);
		std::time_t ban_time = it->second.first;
		int duration_secs = it->second.second;
		if (now - ban_time < duration_secs)
		{
			return true;
		}
		else
		{
			banned_users.erase(it);
		}
	}
	return false;
}

bool Channel::isOperator(User *user) const
{
	for (size_t i = 0; i < operators.size(); i++)
		if (operators[i] == user)
			return (true);
	return (false);
}

std::vector<User *> Channel::getMembers()
{
	std::vector<User *> members = std::vector<User *>();

	for (std::map<int, User *>::iterator it = this->members.begin(); it != this->members.end(); ++it)
		members.push_back(it->second);
	return (members);
}

std::string Channel::getTopic() const { return (topic); }
std::string Channel::getName() const { return (name); }
std::map<std::string, std::pair<std::time_t, int> > Channel::getBannedUsers() const { return banned_users; }
std::vector<User *> Channel::getOperators() const { return (operators); }

// channel Mode
void Channel::setMode(std::string mode)
{
	this->mode = mode;
}

std::string Channel::getMode()
{
	return (mode);
}

// User OP
void Channel::setUserMode(User &user, std::string mode) { userMode[user.getFd()] = mode; }
std::string Channel::getUserMode(User &user) { return (userMode[user.getFd()]); }

// max User pour le channel avec le mode '+l'
void Channel::setMaxUsers(std::string maxUsers) { this->maxUsers = maxUsers; }
std::string Channel::getMaxUsers() { return (maxUsers); }

// key
void Channel::setKey(std::string key) { this->key = key; }
std::string Channel::getKey() { return (key); }

// invitation
void Channel::addInvited(User *user) { invited.push_back(user); }
bool Channel::isInvited(User *user) { return (std::find(invited.begin(), invited.end(), user) != invited.end()); }
void Channel::removeInvited(User *user)
{
	std::vector<User *>::iterator it = std::find(invited.begin(), invited.end(), user);

	if (it != invited.end())
		invited.erase(it);
}


// +i (invite only) : Seuls les utilisateurs invités peuvent rejoindre le canal.
// +k (password) : Un mot de passe est requis pour rejoindre le canal.
// +l (limit) : Définit la limite d'utilisateurs pour le canal.
