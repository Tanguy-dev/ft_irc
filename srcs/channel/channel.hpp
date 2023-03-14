/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 21:42:12 by gadeneux          #+#    #+#             */
/*   Updated: 2023/03/14 00:43:07 by thamon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>
#include <map>

class User;

class Channel
{
public:
	Channel(std::string name);

	void join(User *user);
	void quit(User *user);

	bool ban(User *user, int duration_secs);
	bool unban(User *user);

	void setTopic(std::string const &topic);

	void sendMessage(User *sender, std::string message);

	bool isMember(User *user) const;
	bool isUserBanned(User *user);

	bool isOperator(User *user) const;

	bool op(User *user);
	bool deop(User *user);

	std::string getTopic() const;
	std::string getName() const;
	std::vector<User *> getMembers();
	std::map<std::string, std::pair<std::time_t, int> > getBannedUsers() const;
	std::vector<User *> getOperators() const;

	// channel mode
	void 			setMode(std::string mode);
	std::string 	getMode();

	void			setUserMode(User &user, std::string mode);
	std::string		getUserMode(User &user);

	void setMaxUsers(std::string maxUsers);
	std::string getMaxUsers();

	void setKey(std::string key);
	std::string getKey();

	void addInvited(User *user);
	bool isInvited(User *user);
	void removeInvited(User *user);
private:

	std::string name;
	std::string topic;
	std::map<int, User *> members;
	std::vector<User *> operators;
	std::map<std::string, std::pair<std::time_t, int> > banned_users;
	
	std::string mode;
	std::map<int, std::string> userMode;

	std::string maxUsers;

	std::string key;

	std::vector<User *> invited;
};

#endif