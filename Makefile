# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myassine <myassine@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/06 02:16:52 by myassine          #+#    #+#              #
#    Updated: 2024/02/27 17:32:33 by myassine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Définition des dossiers source et objet
SRCS_DIR = srcs/
OBJ_DIR = .obj/

# Sous-dossiers source
BUILTINS_DIR = $(SRCS_DIR)builtins/
ENV_DIR = $(SRCS_DIR)env/
EXECUTION_DIR = $(SRCS_DIR)execution/
PARSING_DIR = $(SRCS_DIR)parsing/
SIGNALS_DIR = $(SRCS_DIR)signals/
UTILS_DIR = $(SRCS_DIR)utils/

# Fichiers source dans le dossier principal
MAIN_SRCS = minishell.c main_1.c main_2.c

# Fichiers source par sous-dossier
BUILTINS_SRCS = builtins.c builtins_2.c builtins_3.c builtins_4.c builtins_5.c
ENV_SRCS = env.c
EXECUTION_SRCS = mini1.c mini2.c mini3.c mini4.c struct_main.c struct_main2.c redirect_heredoc.c verifcmd.c
PARSING_SRCS = check_char.c check_char_2.c check_char_3.c check_error.c check_quote.c check_quote_2.c \
              check_redir_a_pipe.c expand.c expand_2.c skip_char.c tokenization.c tokenization_2.c tokenization_3.c
SIGNALS_SRCS = signal.c
UTILS_SRCS = free.c free_2.c ft_split_path.c lib_mini.c lib_mini_2.c lib_mini_3.c lib_mini_4.c lib_mini_5.c \
            lstsplit.c malloc.c print.c

# Chemin complet des fichiers source
MAIN_FILES = $(addprefix $(SRCS_DIR), $(MAIN_SRCS))
BUILTINS_FILES = $(addprefix $(BUILTINS_DIR), $(BUILTINS_SRCS))
ENV_FILES = $(addprefix $(ENV_DIR), $(ENV_SRCS))
EXECUTION_FILES = $(addprefix $(EXECUTION_DIR), $(EXECUTION_SRCS))
PARSING_FILES = $(addprefix $(PARSING_DIR), $(PARSING_SRCS))
SIGNALS_FILES = $(addprefix $(SIGNALS_DIR), $(SIGNALS_SRCS))
UTILS_FILES = $(addprefix $(UTILS_DIR), $(UTILS_SRCS))

# Tous les fichiers source
ALL_SRCS = $(MAIN_FILES) $(BUILTINS_FILES) $(ENV_FILES) $(EXECUTION_FILES) $(PARSING_FILES) $(SIGNALS_FILES) $(UTILS_FILES)

# Objets
OBJS = $(patsubst $(SRCS_DIR)%.c,$(OBJ_DIR)%.o,$(MAIN_FILES))
OBJS += $(patsubst $(BUILTINS_DIR)%.c,$(OBJ_DIR)builtins/%.o,$(BUILTINS_FILES))
OBJS += $(patsubst $(ENV_DIR)%.c,$(OBJ_DIR)env/%.o,$(ENV_FILES))
OBJS += $(patsubst $(EXECUTION_DIR)%.c,$(OBJ_DIR)execution/%.o,$(EXECUTION_FILES))
OBJS += $(patsubst $(PARSING_DIR)%.c,$(OBJ_DIR)parsing/%.o,$(PARSING_FILES))
OBJS += $(patsubst $(SIGNALS_DIR)%.c,$(OBJ_DIR)signals/%.o,$(SIGNALS_FILES))
OBJS += $(patsubst $(UTILS_DIR)%.c,$(OBJ_DIR)utils/%.o,$(UTILS_FILES))

# Compilation
CC = gcc
RM = rm -f
CFLAGS = -I. -Wall -Wextra -Werror -g3 #-fsanitize=address
LIBS = -lpthread -lreadline

NAME = minishell

all: 
	@mkdir -p $(OBJ_DIR) $(OBJ_DIR)builtins $(OBJ_DIR)env $(OBJ_DIR)execution $(OBJ_DIR)parsing $(OBJ_DIR)signals $(OBJ_DIR)utils
	@make $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)
	@echo -e Compiled '\033[1;32m' $(NAME) '\033[0m'

$(OBJ_DIR)%.o: $(SRCS_DIR)%.c
	@echo -e Compiling '\033[0;34m $< \033[0m'
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)builtins/%.o: $(BUILTINS_DIR)%.c
	@echo -e Compiling '\033[0;34m $< \033[0m'
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)env/%.o: $(ENV_DIR)%.c
	@echo -e Compiling '\033[0;34m $< \033[0m'
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)execution/%.o: $(EXECUTION_DIR)%.c
	@echo -e Compiling '\033[0;34m $< \033[0m'
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)parsing/%.o: $(PARSING_DIR)%.c
	@echo -e Compiling '\033[0;34m $< \033[0m'
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)signals/%.o: $(SIGNALS_DIR)%.c
	@echo -e Compiling '\033[0;34m $< \033[0m'
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)utils/%.o: $(UTILS_DIR)%.c
	@echo -e Compiling '\033[0;34m $< \033[0m'
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) -r $(OBJ_DIR)
	@echo -e '\033[0;33mRemoved object files\033[0m'

fclean: clean
	@$(RM) $(NAME)
	@echo -e '\033[0;33mRemoved $(NAME)\033[0m'

re:	fclean all

v: $(NAME)
	valgrind --suppressions=./valg.supp --track-fds=yes --leak-check=full --show-leak-kinds=all ./$(NAME)

i: re
	@valgrind --leak-check=full --suppressions=valgrind_leaks_ignore.txt --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log env -i ./$(NAME)

.PHONY: all clean fclean re v i