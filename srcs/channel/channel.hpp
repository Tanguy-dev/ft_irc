/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 21:42:12 by gadeneux          #+#    #+#             */
/*   Updated: 2023/03/27 17:03:03 by thamon           ###   ########.fr       */
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

	// join & quit
	// bool join(User *user, std::string key);
	bool join(User *user);
	void quit(User *user);

	// ban
	bool ban(User *user, int duration_secs);
	bool unban(User *user);
	bool isUserBanned(User *user);

	// op
	bool op(User *user);
	bool deop(User *user);
	bool isOperator(User *user) const;

	// topic
	void setTopic(std::string const &topic);
	std::string getTopic() const;

	// send message
	void sendMessage(User *sender, std::string message);

	// channel mode
	void 			setMode(char mode, bool active);
	bool 			isModeActived(char mode);
	std::string 	getMode();

	// user mode
	void			setUserMode(User &user, std::string mode);
	std::string		getUserMode(User &user);

	// user limitation
	void setMaxUsers(int maxUsers);
	int getMaxUsers();

	// key
	void setKey(std::string key);
	std::string getKey();
	bool isKeyPresent();

	// invite
	void addInvited(User *user);
	bool isInvited(User *user);
	void removeInvited(User *user);
	bool isMember(User *user) const;
	bool isMemberName(std::string nick);
	bool isInviteOnly();
	bool isMemberLimited();
	int getMemberCount();

	std::string getName() const;
	std::vector<User *> getMembers();
	std::map<std::string, std::pair<std::time_t, int> > getBannedUsers() const;
	std::vector<User *> getOperators() const;

private:

	std::string name;
	std::string topic;
	std::map<int, User *> members;
	std::vector<User *> operators;
	std::map<std::string, std::pair<std::time_t, int> > banned_users;
	
	std::string modes;
	std::map<int, std::string> userMode;

	int maxUsers;

	std::string key;

	std::vector<User *> invited;
};

#endif