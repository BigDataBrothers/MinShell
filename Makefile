# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myassine <myassine@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/06 02:16:52 by myassine          #+#    #+#              #
#    Updated: 2023/12/13 17:45:29 by myassine         ###   ########.fr        #
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
	@valgrind --leak-check=full --suppressions=valgrind_leaks_ignore.txt --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log ./minishell

.PHONY:			all clean fclean re valgrind