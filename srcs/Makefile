CXX = c++
CXXFLAGS = -g -Werror -Wextra -Wall -std=c++98 -pedantic-errors -MMD -MP
NAME = server
SRCS = main.cpp Socket.cpp Logger.cpp Kqueue.cpp Client.cpp
CONF_DIR = conf/
CONF_FILES = Location.cpp configParser.cpp virtualServer.cpp 
CONF_SRCS = $(addprefix $(CONF_DIR), $(CONF_FILES))

OBJS = $(SRCS:.cpp=.o)
CONF_OBJS = $(CONF_SRCS:.cpp=.o)
OBJS += $(CONF_OBJS)

all: $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)
	$(RM) *.d

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include *.d
