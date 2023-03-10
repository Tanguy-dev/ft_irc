/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 21:45:22 by gadeneux          #+#    #+#             */
/*   Updated: 2023/03/10 02:47:07 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"
#include "../user/user.hpp"

Channel::Channel(std::string name) : name(name), topic("")
{
	
}

void Channel::join(User *user)
{
	user->sendMessage(*user, "test");
	if (isMember(user))
		throw std::exception();

	members.push_back(user);
}

void Channel::quit(User *user)
{
	if (!isMember(user))
		throw std::exception();

	members.erase(std::find(members.begin(), members.end(), user));
}

void Channel::ban(User *user, int duration_secs)
{
	std::time_t now = std::time(NULL);

	// std::time_t ban_expiry = now + duration_secs;

	banned_users[user->getNickname()] = std::make_pair(now, duration_secs);
}

void Channel::unban(User *user)
{
	std::map<std::string, std::pair<std::time_t, int> >::iterator it = banned_users.find(user->getNickname());
	if (it != banned_users.end())
	{
		banned_users.erase(it);
	}
}

void Channel::setTopic(std::string const &topic)
{
	this->topic = topic;
}

void Channel::sendMessage(User *sender, std::string message)
{
	// for (std::vector<User *>::iterator it = members.begin(); it != members.end(); ++it)
	// 	sender->sendMessage(*(*it), message);
	for (std::vector<User *>::iterator it = members.begin(); it != members.end(); ++it)
	{
		// User *user = *it;
		// if (user != sender)
		// 	sender->sendMessage(*sender, "PRIVMSG #" + name + " :" + message);
		// else
		sender->sendMessage(*(*it), message);
	}
}

bool Channel::isMember(User *user) const
{
    for (size_t i = 0; i < members.size(); i++)
	{
		if (members[i] == user)
			return (true);
	}
	
    return (false);
}

bool Channel::isUserBanned(std::string user_nick)
{
    std::map<std::string, std::pair<std::time_t, int> >::iterator it = banned_users.find(user_nick);
    if (it != banned_users.end()) {
        std::time_t now = std::time(NULL);
        std::time_t ban_time = it->second.first;
        int duration_secs = it->second.second;
        if (now - ban_time < duration_secs) {
            return true;
        }
        else {
            banned_users.erase(it);
        }
    }
    return false;
}

std::string Channel::getTopic() const { return (topic); }
std::string Channel::getName() const { return (name); }
std::vector<User *> Channel::getMembers() const { return (members); }
std::map<std::string, std::pair<std::time_t, int> > Channel::getBannedUsers() const { return banned_users; }