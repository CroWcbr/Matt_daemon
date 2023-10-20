NAME		=	Matt_daemon

CC			=	g++
FLAGS		=	#-Wall -Wextra -Werror

OBJ_DIR		=	obj

SRC			=	./src/main.cpp \
				./src/Matt_daemon.cpp \
				./src/Server.cpp \
				./src/Tintin_reporter.cpp

OBJ			=	$(SRC:%.cpp=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o:	%.cpp
				@mkdir -p $(@D)
				$(CC) $(FLAGS) -c -o $@ $<

RM_DIR		=	rm -rf
RM_FILE		=	rm -f

#COLORS
C_NO		=	"\033[00m"
C_OK		=	"\033[32m"
C_GOOD		=	"\033[32m"

#DEBUG
SUCCESS		=	$(C_GOOD)SUCCESS$(C_NO)
OK			=	$(C_OK)OK$(C_NO)

all:		$(NAME)
DESTDIR = ./obj
$(NAME):	$(OBJ)
			$(CC)  $(FLAGS) $(OBJ) -o $(NAME)
			@echo "\tCompiling...\t" [ $(NAME) ] $(SUCCESS)

bonus:		
			qmake -o Ben_AFK/Makefile Ben_AFK.pro
			$(MAKE) -C test
			cp ./Ben_AFK/Ben_AFK .

clean:
			@${RM_DIR} ${OBJ_DIR}
			@echo "\tCleaning...\t" [ $(NAME) ] $(OK)
# add BenAFK

fclean:		clean
			@${RM_FILE} $(NAME)
			@echo "\tDeleting...\t" [ $(NAME) ] $(OK)

re:			fclean all

.PHONY: all clean fclean re bonus
