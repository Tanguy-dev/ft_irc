NAME				=	ircserv

SRCS				=	srcs
OBJECT				=	objs

SRCS_HPP			=	server/server.hpp			\
						server/config/getParams.hpp	\
						utils/utils.hpp \
						user/user.hpp \
						logger/logger.hpp			\
						server/config/displayConsole.hpp

SRCS_CPP			=	main.cpp					\
						server/server.cpp			\
						server/config/getParams.cpp	\
						utils/utils.cpp \
						user/user.cpp \
						logger/logger.cpp			\
						server/config/displayConsole.cpp

CPP					=	clang++
FLAGS				=	-Wall -Wextra -Werror -std=c++98

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
