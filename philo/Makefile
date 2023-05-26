NAME 			= philo
SOURCES 		= $(shell echo srcs/*.c)
OBJECTS 		= $(SOURCES:.c=.o)
CC 				= gcc
CFLAGS_PROD 	= -g -Wall -Wextra -Werror -O3

# --- COLORS ---

NONE=\033[0m
GRAY=\033[2;37m
RED=\033[31m
GREEN=\033[32m

# --- RULES ---

all: $(NAME)

$(NAME): $(OBJECTS) 
		@echo  "$(GRAY)----Compiling $(NAME)----$(NONE)"
		$(CC) -o $@ $(OBJECTS)
		@echo "$(GREEN)$(NAME) Compiled! ᕦ(♥_♥)ᕤ$(NONE)\n"

.c.o:	%.o : %.c
		$(CC) -c $< -o $@

clean:
		@echo "$(GREEN)Deleting obs! ( ͡° ͜ʖ ͡°) $(NONE)"
		rm -f $(OBJECTS)

fclean:	clean
		@echo "$(RED)Deleting everything! ( ͡° ͜ʖ ͡°) $(NONE)"
		rm -f $(NAME) $(OBJECTS)

show:
		@printf "NAME	: $(NAME)\n"
		@printf "CC	: $(CC)\n"
		@printf "CFLAGS	: $(CFLAGS_PROD)\n"
		@printf "SOURCES	: $(SOURCES)\n"

valgrind: all
		 valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -s ./so_long maps/map_4.ber

rs:		 all
		

re: fclean all

.PHONY: all clean fclean re show valgrind