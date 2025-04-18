/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:36:52 by myassine          #+#    #+#             */
/*   Updated: 2024/03/01 18:01:17 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_echo_1(t_block *block, int *i, int *x, int *check_n)
{
	(*i) = 1;
	if (block->arg[(*x)][0] != '-' \
	|| (block->arg[(*x)][1] && block->arg[(*x)][1] != 'n') \
	|| !block->arg[(*x)][1])
		return (0);
	while (block->arg[(*x)][(*i)])
	{
		if (block->arg[(*x)][(*i)] != 'n')
			return ((*check_n) = 0, 0);
		(*check_n)++;
		(*i)++;
	}
	if (block->arg[(*x)][(*i)] && block->arg[(*x)][(*i)] != 'n')
		return (0);
	(*x)++;
	return (1);
}

void	ft_echo(t_block *block)
{
	int	x;
	int	i;
	int	check_n;

	x = 0;
	check_n = 0;
	if (!block->arg)
		return (printf("\n"), (void) NULL);
	while (block->arg[x])
		if (!ft_echo_1(block, &i, &x, &check_n))
			break ;
	i = len_tab(block->arg);
	while (block->arg[x])
	{
		if (write(1, block->arg[x], ft_strlen(block->arg[x])) == -1)
			write (2, \
			"minishell: echo: write error: No space left on device\n", 55);
		if (x < i - 1)
			write(1, " ", 1);
		x++;
	}
	if (!check_n)
		printf("\n");
}

void	ft_putstr_fd(char *s, int fd)
{
	if (*s)
		write(fd, s, ft_strlen(s));
}

void	ft_pwd(t_block *block)
{
	char	*path;

	if (!ft_strcmp(block->cmd, "pwd"))
	{
		path = get_current_directory_with_prompt();
		printf("%s\n", path);
	}
}
