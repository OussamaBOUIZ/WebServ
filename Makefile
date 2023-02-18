NAME=webserv
CC=c++
CPPFLAGS= -Wall -Wextra -Werror -std=c++98
RM=rm -rf
SOURCES = _main.cpp ConfigFileParser.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJECTS)
	$(CC) $(CPPFLAGS) $(OBJECTS) -o $(NAME)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(NAME)

re : fclean all