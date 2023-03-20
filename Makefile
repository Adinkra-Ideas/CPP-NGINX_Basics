NAME	=	webserv

SRCS		=	main.cpp ConfigParser.cpp Server.cpp ServerManager.cpp Client.cpp \
				Request.cpp Response.cpp utils.cpp Location.cpp Mime.cpp

OBJDIR = build/
SRCDIR = src/
HEADER = includes/

OBJS := $(SRCS:.cpp=.o)
SRCS_PREFIXED := $(addprefix $(SRCDIR), $(SRCS))
OBJECTS_PREFIXED := $(addprefix $(OBJDIR), $(OBJS))
CXX			=	c++

CXXFLAGS	=	-Wall -Werror -Wextra -std=c++98 -pedantic -g3 -fsanitize=address

all:		$(NAME)

$(NAME): $(OBJECTS_PREFIXED) $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS_PREFIXED) -o $(NAME)
	@echo "$(NAME): $(GREEN)$(NAME) was created$(RESET)"

$(OBJECTS_PREFIXED): build/%.o : src/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -I$(HEADER) $< -o $@ 

clean:		
			/bin/rm -rf $(OBJDIR)
				
fclean:		clean
			/bin/rm -f $(NAME)
			clear

re:			fclean	all

.PHONY:		all	clean	fclean	re
