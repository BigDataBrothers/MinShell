# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mgamil <mgamil@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/06 02:16:52 by myassine          #+#    #+#              #
#    Updated: 2023/12/26 19:01:42 by mgamil           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS			= 	minishell.c \
					check_char.c \
					check_char_2.c \
					check_quote.c \
					check_error.c \
					check_redir_a_pipe.c \
					skip_char.c \
					free.c \
					print.c \
					malloc.c \
					expand.c \
					builtins.c \
					lib_mini.c \
					ft_split_path.c \
					tokenization.c \
					norm/mini1.c \
					norm/mini2.c \
					norm/mini3.c \
					norm/mini4.c \
					norm/lstsplit.c \
					norm/verifcmd.c \
					env.c

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

v: re
	valgrind --leak-check=full --suppressions=valgrind_leaks_ignore.txt --show-leak-kinds=all --trace-children=yes --track-origins=yes --log-file=valgrind.log ./minishell

i: re
	@valgrind --leak-check=full --suppressions=valgrind_leaks_ignore.txt --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log env -i ./minishell
.PHONY:			all clean fclean re valgrind