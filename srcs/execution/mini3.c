/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:07:09 by myassine          #+#    #+#             */
/*   Updated: 2024/03/03 21:14:05 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	redirect_input(char *filename, t_all *all)
{
	int	file;

	file = open(filename, O_RDONLY);
	if (file == -1)
	{
		close(all->saved_stdin);
		close_saved(all);
		print_error_2("minishell: ", filename, ": No such file or directory");
		close(all->pipe_fds[0]);
		close(all->pipe_fds[1]);
		if (all->pid != -1)
		{
			freeme(all, 1);
			exit(EXIT_FAILURE);
		}
		all->i_a = -1;
		all->j_a = -1;
		return (1);
	}
	dup2(file, STDIN_FILENO);
	close(file);
	return (0);
}

void	apply_redirections_to_command_line(t_all *all)
{
	t_dir	*tmp;

	tmp = all->test->dir;
	while (tmp)
	{
		tmp->file = if_quote(tmp->file);
		if (tmp->type == HEREDOC \
			&& redirect_heredoc(tmp->file, all->saved_stdin, all))
			break ;
		else if (tmp->type == APPEND && append_output(tmp->file, all))
			break ;
		else if (tmp->type == IN && (redirect_input(tmp->file, all)))
			break ;
		else if (tmp->type == OUT && (redirect_output(tmp->file, all)))
			break ;
		tmp = tmp->next;
	}
	close(all->saved_stdin);
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
	if ((block->arg && len_tab(block->arg) != 1) \
	|| (block->arg && !is_real_num(block->arg[0])))
		return (write(2, "exit doesn't have the right arguments\n", 39), 0);
	write(2, "exit\n", 5);
	return (1);
}
