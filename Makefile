# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myassine <myassine@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/06 02:16:52 by myassine          #+#    #+#              #
#    Updated: 2024/02/23 16:41:45 by myassine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_DIR = srcs/

SRCS			= 	minishell.c \
					main_1.c \
					struct_main.c \
					struct_main2.c \
					check_quote.c \
					check_quote_2.c \
					redirect_heredoc.c\
					lib_mini.c \
					lib_mini_2.c \
					lib_mini_3.c \
					lib_mini_4.c \
					ft_split_path.c \
					tokenization.c \
					tokenization_2.c \
					tokenization_3.c \
					verifcmd.c \
					free.c \
					print.c \
					mini1.c \
					mini2.c \
					mini3.c \
					mini4.c \
					malloc.c \
					expand.c \
					expand_2.c \
					lstsplit.c \
					builtins.c \
					builtins_2.c \
					builtins_3.c \
					builtins_4.c \
					builtins_5.c \
					skip_char.c \
					check_error.c \
					check_char.c \
					check_char_2.c \
					check_char_3.c \
					check_redir_a_pipe.c \
					env.c
 
SRCS_PATH = $(addprefix $(SRCS_DIR), $(SRCS))

OBJ_DIR 		= .obj/
OBJS			= $(SRCS:.c=.o)
OBJS_PATH = $(addprefix $(OBJ_DIR), $(OBJS))


CC				= gcc
RM				= rm -f
CFLAGS			= -I. -Wall -Wextra -Werror -g3 -g #-fsanitize=address
LIBS			= -lpthread -lreadline

NAME			= minishell

all:			
	@mkdir -p $(OBJ_DIR)
	make $(NAME)

$(NAME):		$(OBJS_PATH) $(SRCS_PATH)
				@gcc ${CFLAGS} -o ${NAME} ${OBJS_PATH} ${LIBS}
				@echo -e Compiled '\033[1;32m' $(NAME) '\033[0m'

$(OBJ_DIR)%.o: $(SRCS_DIR)%.c
	@echo -e Compiling '\033[0;34m $< \033[0m'
	@gcc $(CFLAGS) -c $< -o $@ 
	

clean:
				@$(RM) $(OBJ_DIR)* $(BONUS_OBJS)
				@echo -e '\033[0;33m$(RM) $(OBJ_DIR)* $(BONUS_OBJS) \033[0m'

fclean:			clean
				@$(RM) $(NAME)
				@echo -e '\033[0;33m$(RM) $(NAME) \033[0m'

re:				fclean $(NAME)

v: $(NAME)
	valgrind --suppressions=./valg.supp --track-fds=yes --leak-check=full --show-leak-kinds=all ./minishell
i: re
	@valgrind --leak-check=full --suppressions=valgrind_leaks_ignore.txt --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log env -i ./minishell
.PHONY:			all clean fclean re valgrind