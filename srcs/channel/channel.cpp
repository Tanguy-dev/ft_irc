/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 21:45:22 by gadeneux          #+#    #+#             */
/*   Updated: 2023/03/30 18:25:15 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"
#include "../user/user.hpp"

Channel::Channel(std::string name) : name(name), topic(""), modes(""), maxUsers(0) {}

bool Channel::join(User *user)
{
	members[user->getFd()] = user;
	return (true);
}

void Channel::quit(User *user)
{
	if (!isMember(user))
		throw std::exception();

	deop(user);
	members.erase(members.find(user->getFd()));
}

bool Channel::isKeyPresent()
{
	return (isModeActived('k'));
}

bool Channel::isMemberLimited()
{
	return (isModeActived('l'));
}

bool Channel::isInviteOnly()
{
	return (isModeActived('l'));
}

int Channel::getMemberCount()
{
	return (getMembers().size());
}

bool Channel::ban(User *user, int delay_in_seconds)
{
	if (isUserBanned(user))
		return (false);

	std::time_t now = std::time(NULL);

	banned_users[user->getHostname()] = std::make_pair(now, delay_in_seconds);
	return (true);
}

bool Channel::unban(User *user)
{
	if (!isUserBanned(user))
		return (false);

	std::map<std::string, std::pair<std::time_t, int> >::iterator it = banned_users.find(user->getHostname());
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
	return (members.find(user->getFd()) != members.end());
}

bool Channel::isMemberName(std::string nick)
{
	for (std::map<int, User *>::iterator it = members.begin(); it != members.end(); ++it)
	{
		
		if (nick == (*it).second->getNickname())
			return (true);
	}
	return (false);
}

bool Channel::isUserBanned(User *user)
{
	std::map<std::string, std::pair<std::time_t, int> >::iterator it = banned_users.find(user->getHostname());
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

void Channel::setMode(char mode, bool active)
{
    if (active) {
        // Ajoute le mode si il n'est pas déjà présent
        if (modes.find(mode) == std::string::npos) {
            modes += mode;
        }
    } else {
        // Supprime le mode s'il est présent
        size_t index = modes.find(mode);
        if (index != std::string::npos) {
            modes.erase(index, 1);
        }
    }
}

std::string Channel::getMode()
{
	return (modes);
}

bool Channel::isModeActived(char mode)
{
	return (modes.find(mode) != std::string::npos);
}

// User OP
void Channel::setUserMode(User &user, std::string mode) { userMode[user.getFd()] = mode; }
std::string Channel::getUserMode(User &user) { return (userMode[user.getFd()]); }

// max User pour le channel avec le mode '+l'
void Channel::setMaxUsers(int maxUsers) { this->maxUsers = maxUsers; }
int Channel::getMaxUsers() { return (maxUsers); }

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