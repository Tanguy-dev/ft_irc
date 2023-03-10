/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamon <thamon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 21:42:12 by gadeneux          #+#    #+#             */
/*   Updated: 2023/03/10 02:23:36 by thamon           ###   ########.fr       */
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
        
		void ban(User * user, int duration_secs);
        void unban(User * user);
        
        void setTopic(std::string const & topic);
        
        void sendMessage(User * sender, std::string message);
        
        bool isMember(User * user) const;
        bool isUserBanned(std::string user_nick);

		bool isOperator(User *user) const;

		void op(User *user);
		void deop(User *user);
        
        std::string getTopic() const;
        std::string getName() const;
        std::vector<User *> getMembers() const;
        std::map<std::string, std::pair<std::time_t, int> > getBannedUsers() const;

    private:
    
        std::string name;
        std::string topic;
        std::vector<User *> members;
        std::map<std::string, std::pair<std::time_t, int> > banned_users;
};

#endif