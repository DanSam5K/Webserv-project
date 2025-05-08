NAME = webserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_DIRS = main.cpp \
		config/ConfigParser.cpp \
		server/Server.cpp \
		http/Request.cpp http/Response.cpp \
		utils/Logger.cpp \
		cgi/CGIHandler.cpp

OBJS = $(SRC_DIRS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
