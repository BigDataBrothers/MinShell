/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:07:13 by myassine          #+#    #+#             */
/*   Updated: 2024/02/23 17:09:26 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>

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

void	all_free_1(t_block *test, t_env *env, t_env *head_env, char **args)
{
	if (test)
		free_block_list(test);
	(void)env;
	(void)head_env;
	(void)args;
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

void	redirect_output(char *filename, t_all *all)
{
	int	file;

	file = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (file == -1)
	{
		perror(filename);
		close_saved(all);
		exit(EXIT_FAILURE);
	}
	if (dup2(file, STDOUT_FILENO) == -1)
	{
		write(2, "Erreur lors de la redirection de la sortie standard\n", 53);
		close(file);
		exit(EXIT_FAILURE);
	}
	close(file);
}

void	append_output(char *filename, t_all *all)
{
	int	file;

	file = open(filename, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (file == -1)
	{
		perror(filename);
		close_saved(all);
		exit(EXIT_FAILURE);
	}
	if (dup2(file, STDOUT_FILENO) == -1)
	{
		write(2, "Erreur lors de la redirection de la sortie standard\n", 53);
		close(file);
		exit(EXIT_FAILURE);
	}
	close(file);
}
