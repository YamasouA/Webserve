CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -pedantic-errors -MMD -MP

SRCS = Config.cpp VirtualServer.cpp Location.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)
NAME = config_parser

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)
	rm -f $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
