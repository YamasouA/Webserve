CXX = c++
CXXFLAGS = -g -Werror -Wextra -Wall -std=c++98 -pedantic-errors -MMD -MP
NAME = server
SRCS = main.cpp Socket.cpp Logger.cpp
OBJS = $(SRCS:.cpp=.o)

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
