NAME		= philo

CC			= cc
CFLAGS		= -Wall -Wextra -Werror 

SRC_FILES	= main.c \
			  parsing.c \
			  init.c \
			  init_philos.c \
			  monitoring.c \
			  monitoring_utils.c \
			  philosopher.c \
			  forks.c \
			  utils.c \
			  utils2.c

OBJ_DIR		= obj/
OBJ_FILES	= $(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))

INCLUDES	= -I./

GREEN		= \033[0;32m
YELLOW		= \033[0;33m
RESET		= \033[0m


all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(YELLOW)Compiling: $<$(RESET)"

$(NAME): $(OBJ_DIR) $(OBJ_FILES)
	@$(CC) $(CFLAGS) $(PTHREAD) $(OBJ_FILES) -o $(NAME)
	@echo "$(GREEN)Philosophers compiled successfully!$(RESET)"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)Object files removed.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(YELLOW)Executable removed.$(RESET)"

re: fclean all


.PHONY: clean
