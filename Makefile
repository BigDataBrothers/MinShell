# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myassine <myassine@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/06 02:16:52 by myassine          #+#    #+#              #
#    Updated: 2024/02/05 20:42:14 by myassine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS			= 	minishell.c \
					norm/main_1.c \
					norm/struct_main.c \
					norm/struct_main2.c \
					norm/check_quote.c \
					norm/check_quote_2.c \
					norm/redirect_heredoc.c\
					norm/lib_mini.c \
					norm/lib_mini_2.c \
					norm/lib_mini_3.c \
					norm/lib_mini_4.c \
					norm/ft_split_path.c \
					norm/tokenization.c \
					norm/tokenization_2.c \
					norm/tokenization_3.c \
					norm/verifcmd.c \
					norm/free.c \
					norm/print.c \
					norm/mini1.c \
					norm/mini2.c \
					norm/mini3.c \
					norm/mini4.c \
					norm/malloc.c \
					norm/expand.c \
					norm/expand_2.c \
					norm/lstsplit.c \
					norm/builtins.c \
					norm/builtins_2.c \
					norm/builtins_3.c \
					norm/builtins_4.c \
					norm/builtins_5.c \
					norm/skip_char.c \
					norm/check_error.c \
					norm/check_char.c \
					norm/check_char_2.c \
					norm/check_char_3.c \
					norm/check_redir_a_pipe.c \
					norm/env.c

OBJS			= $(SRCS:.c=.o)

CC				= gcc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror -g3 -g #-fsanitize=address
LIBS			= -lpthread -lreadline

NAME			= minishell

all:			$(NAME)

$(NAME):		$(OBJS)
				gcc ${CFLAGS} -o ${NAME} ${OBJS} ${LIBS}

clean:
				$(RM) $(OBJS) $(BONUS_OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

v: $(NAME)
	valgrind --leak-check=full --suppressions=valgrind_leaks_ignore.txt --show-leak-kinds=all --track-fds=yes ./minishell

i: re
	@valgrind --leak-check=full --suppressions=valgrind_leaks_ignore.txt --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log env -i ./minishell
.PHONY:			all clean fclean re valgrind