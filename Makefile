NAME        = philo
CC          = cc
CFLAGS      = -Wall -Wextra -Werror

SRC_FILES   = main.c \
              parsing.c \
              init.c \
              init_philos.c \
              monitoring.c \
              monitoring_utils.c \
              philosopher.c \
              forks.c \
              utils.c \
              utils2.c

OBJ_DIR     = obj/
OBJ_FILES   = $(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))

GREEN       = \033[0;32m
YELLOW      = \033[0;33m
BLUE        = \033[0;34m
CYAN        = \033[0;36m
RESET       = \033[0m

all: $(OBJ_DIR) $(NAME)
	@echo "$(CYAN) ██████╗ ██╗  ██╗██╗██╗      ██████╗ ███████╗ ██████╗ ██████╗ ██╗  ██╗███████╗██████╗ ███████╗"
	@echo " ██╔══██╗██║  ██║██║██║     ██╔═══██╗██╔════╝██╔═══██╗██╔══██╗██║  ██║██╔════╝██╔══██╗██╔════╝"
	@echo " ██████╔╝███████║██║██║     ██║   ██║███████╗██║   ██║██████╔╝███████║█████╗  ██████╔╝███████╗"
	@echo " ██╔═══╝ ██╔══██║██║██║     ██║   ██║╚════██║██║   ██║██╔═══╝ ██╔══██║██╔══╝  ██╔══██╗╚════██║"
	@echo " ██║     ██║  ██║██║███████╗╚██████╔╝███████║╚██████╔╝██║     ██║  ██║███████╗██║  ██║███████║"
	@echo " ╚═╝     ╚═╝  ╚═╝╚═╝╚══════╝ ╚═════╝ ╚══════╝ ╚═════╝ ╚═╝     ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝$(RESET)"
	@echo "                                                                           by mtarza (c) 2025"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "$(BLUE)Created object directory$(RESET)"

$(OBJ_DIR)%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(BLUE)Compiling: $<$(RESET)"

$(NAME): $(OBJ_FILES)
	@$(CC) $(CFLAGS) $(OBJ_FILES) -o $(NAME)
	@echo "$(GREEN)Philosophers compiled successfully!$(RESET)"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)Object files removed.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(YELLOW)Executable removed.$(RESET)"

re: fclean all

.PHONY: all clean fclean re