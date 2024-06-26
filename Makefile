NAME		=	Matt_daemon

CC			=	g++
FLAGS		=	-Wall -Wextra -Werror

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

$(NAME):	$(OBJ)
			$(CC) $(FLAGS) $(OBJ) -o $(NAME)
			@echo "\tCompiling...\t" [ $(NAME) ] $(SUCCESS)

client:		
			qmake -o src_client/Makefile ./src_client/src_client.pro
			$(MAKE) -C src_client
			cp -f src_client/Ben_AFK .
			@echo "\tCompiling...\t" [ "Ben_AFK" ] $(SUCCESS)

clean:
			@${RM_DIR} ${OBJ_DIR}
			@echo "\tCleaning...\t" [ $(NAME) ] $(OK)
			@if [ -f src_client/Makefile ]; then \
				$(MAKE) -C src_client clean; \
			fi
			@echo "\tCleaning...\t" [ "client" ] $(OK)

fclean:		clean
			@${RM_FILE} $(NAME)
			@echo "\tDeleting...\t" [ $(NAME) ] $(OK)
			@rm -f src_client/Makefile
			@rm -f src_client/Ben_AFK
			@rm -f src_client/.qmake.stash
			@rm -f Ben_AFK
			@echo "\tDeleting...\t" [ "client" ] $(OK)

re:			fclean all

.PHONY: all clean fclean re bonus
