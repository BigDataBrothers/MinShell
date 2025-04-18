/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 17:47:27 by myassine          #+#    #+#             */
/*   Updated: 2024/03/01 20:25:32 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	redirect_heredoc_2(int *heredoc_file, t_all *all)
{
	(*heredoc_file) = open("heredoc_temp_file.txt", \
	O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if ((*heredoc_file) == -1)
	{
		write(2, "Erreur lors de la création du fichier heredoc\n", 48);
		if (all->pid != -1)
		{
			freeme(all, 1);
			exit(EXIT_FAILURE);
		}
		return (1);
	}
	return (0);
}

void	sigdoc(int sig)
{
	if (sig == SIGINT)
	{
		close(0);
		g_ctrl_c = 130;
	}
}

void	sigdoc2(int sig)
{
	if (sig == SIGINT)
	{
		g_ctrl_c = 130;
		write(2, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		exit(130);
	}
}

int	redirect_heredoc(char *delimiter, int saved_stdin, t_all *all)
{
	char	*input;
	int		heredoc_file;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sigdoc);
	if (redirect_heredoc_2(&heredoc_file, all))
		return (1);
	dup2(saved_stdin, STDIN_FILENO);
	while (g_ctrl_c != 130)
	{
		input = readline("> ");
		if (!input || ft_strcmp(input, delimiter) == 0)
		{
			free(input);
			break ;
		}
		input = expa_chang(input, all);
		ft_putstr_fd(input, heredoc_file);
		ft_putstr_fd("\n", heredoc_file);
		free(input);
	}
	dup2(STDIN_FILENO, saved_stdin);
	close(heredoc_file);
	redirect_input("heredoc_temp_file.txt", all);
	return (0);
}
