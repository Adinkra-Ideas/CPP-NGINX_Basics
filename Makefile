NAME	=	webserv

SRCS		=	main.cpp ConfigParser.cpp ServerConfig.cpp ServerManager.cpp

OBJDIR = build/
SRCDIR = src/
HEADER = inc/

OBJS := $(SRCS:.cpp=.o)
SRCS_PREFIXED := $(addprefix $(SRCDIR), $(SRCS))
OBJECTS_PREFIXED := $(addprefix $(OBJDIR), $(OBJS))
CXX			=	c++

CXXFLAGS	=	-Wall -Werror -Wextra -std=c++98 -pedantic -g3

all:		$(NAME)

$(NAME): $(OBJECTS_PREFIXED) $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS_PREFIXED) -o $(NAME)
	@echo "$(NAME): $(GREEN)$(NAME) was created$(RESET)"

$(OBJECTS_PREFIXED): build/%.o : src/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -I$(HEADER) $< -o $@ 

clean:		
			/bin/rm -f $(OBJ)
				
fclean:		clean
			/bin/rm -f $(NAME)

re:			fclean	all

.PHONY:		all	clean	fclean	re
