NAME 			= philo
SOURCES 		= $(shell echo srcs/*.c)
OBJECTS 		= $(addprefix objects/,$(notdir $(SOURCES:.c=.o)))
CC 				= gcc
CFLAGS 			= -Wall -Wextra -Werror -g -pthread -fsanitize=thread

# --- COLORS ---

NONE=\033[0m
GRAY=\033[2;37m
RED=\033[31m
GREEN=\033[32m

# --- RULES ---

all: $(NAME)

$(NAME): $(OBJECTS) 
	@echo  "$(GRAY)----Compiling $(NAME)----$(NONE)"
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)
	@echo "$(GREEN)$(NAME) Compiled! ᕦ(♥_♥)ᕤ$(NONE)\n"

objects/%.o: srcs/%.c | objects
	$(CC) -c $< -o $@

clean:
	@echo "$(GREEN)Deleting obs! ( ͡° ͜ʖ ͡°) $(NONE)"
	rm -f $(OBJECTS)

fclean:	clean
	@echo "$(RED)Deleting everything! ( ͡° ͜ʖ ͡°) $(NONE)"
	rm -f $(NAME)

re: fclean all

rs:	re
	./philo

.PHONY: all clean fclean re rs show valgrind