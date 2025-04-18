/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:07:13 by myassine          #+#    #+#             */
/*   Updated: 2024/03/01 16:18:35 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	sig_antislash(int sig)
{
	if (sig == SIGQUIT)
		exit(131);
	if (sig == SIGINT)
		exit(130);
}

void	rsigint_handle(int sig)
{
	if (sig == SIGINT)
	{
		g_ctrl_c = 130;
		write(2, "\n", 1);
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	sigint_handler_fork(int sig)
{
	if (sig == SIGINT)
	{
		g_ctrl_c = 130;
		rl_replace_line("", 0);
		rl_redisplay();
		exit(130);
	}
}

int	redirect_output(char *filename, t_all *all)
{
	int		file;

	file = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (file == -1 || dup2(file, STDOUT_FILENO) == -1)
	{
		if (file == -1)
			perror(filename);
		else
			write(2, \
			"Erreur lors de la redirection de la sortie standard\n", 53);
		close_saved(all);
		if (all->pid != -1)
		{
			freeme(all, 1);
			exit(EXIT_FAILURE);
		}
		return (1);
	}
	close(file);
	return (0);
}

int	append_output(char *filename, t_all *all)
{
	int	file;

	file = open(filename, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (file == -1 || dup2(file, STDOUT_FILENO) == -1)
	{
		if (file == -1)
			perror(filename);
		else
			write(2, \
			"Erreur lors de la redirection de la sortie standard\n", 53);
		close_saved(all);
		if (all->pid != -1)
		{
			freeme(all, 1);
			exit(EXIT_FAILURE);
		}
		return (1);
	}
	close(file);
	return (0);
}
