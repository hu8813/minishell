# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/07 19:33:09 by hakaraka          #+#    #+#              #
#    Updated: 2023/08/22 20:27:02 by huaydin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Check if readline library is installed on Linux or Mac, if not show message
CURRENT_OS = $(uname -s)
ifeq ($(CURRENT_OS), Linux)
check_readline:
	@test -f /usr/include/readline/readline.h || { echo "Readline library \ 
		not found. Please install it using the following command:"; \
		echo "sudo apt-get install make libreadline-dev"; \
		exit 1; }
else ifeq ($(CURRENT_OS), Darwin)
	READLINE_DIR := $(shell if [ -d ~/.brew/opt/readline ]; then \
		echo ~/.brew/opt/readline; \
		else if [ -d /usr/local/opt/readline ]; \
		then echo /usr/local/opt/readline; \
		else echo "Readline library not found. Please install it via"; \
			echo "brew install readline"; \
			exit 1; \
		fi; fi)
	INCS = -I $(READLINE_DIR)/include
	LIBS = -L $(READLINE_DIR)/lib
endif

NAME 		= minishell
CFLAGS 		= -Wall -Wextra -Werror
CC 			= cc
SRC_DIR 	= src
OBJ_DIR 	= obj
LIBFT_DIR 	= libft
LIBFT 		= libft/libft.a
RM 			= rm -rf

SRC = 	handle_errors.c   	process_input.c		expand.c      		\
    	builtins_pwd.c    	utils.c           	handle_signals.c  	\
		find_path.c   		builtins_echo.c   	check_if_builtin.c  \
		main.c            	heredoc.c		 	create_fds.c  		\
		builtins_export.c 	utils_free.c 		parse_nodes.c      	\
    	builtins_exit.c   	execute.c         	utils_array.c		\
		builtins_cd.c 		builtins_env.c		trim_quotes.c		\
		parse_split_cmd.c	split_with_space.c	ft_gnl.c

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ_DIR) $(OBJ)
	@echo "Compiling minishell..." 
	@$(CC) $(INCS) $(LIBS)  $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT): 
	@echo "Compiling libft..."
	@make bonus -C libft

clean: 
	@echo "Cleaning..."
	@make clean -C libft
	@$(RM) $(OBJ_DIR)

fclean: clean
	@echo "Fcleaning..."
	$(RM) $(NAME)
	$(RM) $(LIBFT)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

re: fclean
	@make all

.PHONY: all clean fclean re
