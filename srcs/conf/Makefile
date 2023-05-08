CXX = c++
CXXFLAGS = -g -Werror -Wextra -Wall -std=c++98 -pedantic-errors -MMD -MP
NAME = configparser
SRCS = Location.cpp configParser.cpp virtualServer.cpp #main.cpp
TEST_SRCS = test.cpp
OBJS = $(SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

ifdef TEST
OBJS += $(TEST_OBJS)
endif

all: $(NAME)

test:
	make TEST=1

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
