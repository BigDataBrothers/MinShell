/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:07:09 by myassine          #+#    #+#             */
/*   Updated: 2024/01/12 19:59:44 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	redirect_input(char *filename)
{
	int	file;

	file = open(filename, O_RDONLY);
	if (file == -1)
	{
		printf("minishell: %s: No such file or directory\n", filename);
		exit(EXIT_FAILURE);
	}
	dup2(file, STDIN_FILENO);
	close(file);
}

void	apply_redirections_to_command_line(t_all *all)
{
	while (all->test && all->test->dir && all->test->dir->file)
	{
		if (all->test->dir->type == APPEND)
			append_output(all->test->dir->file);
		else if (all->test->dir->type == IN)
			redirect_input(all->test->dir->file);
		else if (all->test->dir->type == OUT)
			redirect_output(all->test->dir->file);
		else if (all->test->dir->type == HEREDOC)
			redirect_heredoc(all->test->dir->file, all->saved_stdin, all);
		all->test->dir = all->test->dir->next;
	}
}

int	is_bultin(char *args)
{
	if (!ft_strcmp(args, "cd"))
		return (1);
	else if (!ft_strcmp(args, "exit"))
		return (1);
	else if (!ft_strcmp(args, "pwd"))
		return (1);
	else if (!ft_strcmp(args, "echo"))
		return (1);
	else if (!ft_strcmp(args, "unset"))
		return (1);
	else if (!ft_strcmp(args, "env"))
		return (1);
	else if (!ft_strcmp(args, "export"))
		return (1);
	return (0);
}

int	is_real_num(const char *num)
{
	int	i;

	i = 0;
	if (num && num[0] != '-' && !is_num(num[0]))
		return (0);
	if (num[0] == '-')
		i++;
	while (num[i])
	{
		if (!is_num(num[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit_1(t_block *block)
{
	dprintf(2, "exit\n");
	if (block->arg && len_tab(block->arg) > 1 && !is_real_num(block->arg[1]))
		return (printf("exit doesn't have the right arguments\n"), 0);
	return (1);
}
