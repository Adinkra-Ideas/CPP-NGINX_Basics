NAME	=	webserv

SRC		=	main.cpp	src/Server.cpp	src/Location.cpp	\
			src/ConfigParser.cpp

OBJ			=	$(SRC:.cpp=.o)

CXX			=	c++

CXXFLAGS	=	-Wall -Werror -Wextra	-std=c++98

all:		$(NAME)

$(NAME):	$(OBJ)
			$(CXX)	$(CXXFLAGS) $(OBJ)	-o $(NAME)

clean:		
			/bin/rm -f $(OBJ)
			/bin/clear
				
fclean:		clean
			/bin/rm -f $(NAME)
			/bin/clear

re:			fclean	all

.PHONY:		all	clean	fclean	re
