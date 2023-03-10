NAME				=	ircserv

SRCS				=	srcs
OBJECT				=	objs

SRCS_HPP			=	server/server.hpp			\
						server/config/getParams.hpp	\
						utils/utils.hpp \
						user/user.hpp \
						logger/logger.hpp			\
						server/config/displayConsole.hpp	\
						commands/commands.hpp				\
						channel/channel.hpp

SRCS_CPP			=	main.cpp					\
						channel/channel.cpp			\
						server/server.cpp			\
						server/config/getParams.cpp	\
						utils/utils.cpp \
						user/user.cpp \
						logger/logger.cpp			\
						server/config/displayConsole.cpp	\
						commands/commands.cpp				\
						commands/RPL.cpp				\
						commands/commands_name/connection/PASS.cpp	\
						commands/commands_name/connection/NICK.cpp	\
						commands/commands_name/connection/USER.cpp	\
						commands/commands_name/connection/QUIT.cpp	\
						commands/commands_name/channel/JOIN.cpp	\
						commands/commands_name/check/PING.cpp	\
						commands/commands_name/check/PONG.cpp	\
						commands/commands_name/user/PRIVMSG.cpp	\
						commands/commands_name/moderation/BAN.cpp	\
						commands/commands_name/moderation/BANLIST.cpp	\
						commands/commands_name/moderation/CLEAR.cpp	\
						commands/commands_name/moderation/DEOP.cpp	\
						commands/commands_name/moderation/INVITE.cpp	\
						commands/commands_name/moderation/KICK.cpp	\
						commands/commands_name/moderation/KICKALL.cpp	\
						commands/commands_name/moderation/KICKBAN.cpp	\
						commands/commands_name/moderation/OP.cpp	\
						commands/commands_name/moderation/TOPIC.cpp	\
						commands/commands_name/moderation/UNBAN.cpp	\
						commands/commands_name/moderation/WHOIS.cpp

CPP					=	clang++
FLAGS				=	-Wall -Wextra -Werror -std=c++98 -g

ARGUMENTS			=	6667 password

all: $(NAME)

$(OBJECT)/%.o: $(SRCS)/%.cpp $(SRCS_HPP:%=$(SRCS)/%)
	@printf "\e[33m"
	@printf "Compile\t$< -> $@\n"
	mkdir -p $(dir $@)
	$(CPP) $(FLAGS) -c $< -o $@

$(NAME): $(SRCS_CPP:%.cpp=$(OBJECT)/%.o)
	@printf "\e[32m"
	@printf "Build\t$@\n"
	$(CPP) -o $@ $(SRCS_CPP:%.cpp=$(OBJECT)/%.o)
	@printf "\033[1;37m"

start: all
	@printf "\e[0m"
	./$(NAME) $(ARGUMENTS)

clean:
	@printf "\e[31m"
	@printf "Remove\t$(OBJECT)\n"
	rm -rf $(OBJECT)

fclean: clean
	@printf "Remove\t$(NAME)\n"
	rm -f $(NAME)

re: fclean all

.PHONY: all start clean fclean re
