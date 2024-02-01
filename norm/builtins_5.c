/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:36:52 by myassine          #+#    #+#             */
/*   Updated: 2024/01/12 18:04:36 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_echo_1(t_block *block, int *i, int *x, int *check_n)
{
	(*i) = 1;
	if (block->arg[(*x)][0] != '-')
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
		return ;
	while (block->arg[x])
		if (!ft_echo_1(block, &i, &x, &check_n))
			break ;
	i = len_tab(block->arg);
	while (block->arg[x])
	{
		printf("%s", block->arg[x]);
		if (x < i - 1)
			printf(" ");
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
