CXX = c++
CXXFLAGS = -g -Werror -Wextra -Wall -std=c++98 -pedantic-errors -MMD -MP
NAME = server
SRCS = httpParser.cpp httpReq.cpp test.cpp

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
