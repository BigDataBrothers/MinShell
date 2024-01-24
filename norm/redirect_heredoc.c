/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-tena <ade-tena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 17:47:27 by myassine          #+#    #+#             */
/*   Updated: 2024/01/19 15:24:09 by ade-tena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	redirect_heredoc_2(int *heredoc_file)
{
	(*heredoc_file) = open("heredoc_temp_file.txt", \
	O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if ((*heredoc_file) == -1)
	{
		perror("Erreur lors de la création du fichier heredoc");
		exit(EXIT_FAILURE);
	}
}

void	redirect_heredoc_3(t_all *all)
{
	perror("Erreur lors de l'écriture dans le fichier heredoc");
	all_free_1(all->test, all->env, all->head_env, all->args);
	exit(EXIT_FAILURE);
}

void	redirect_heredoc(char *delimiter, int saved_stdin, t_all *all)
{
	char	*input;
	int		heredoc_file;
	size_t	input_len;

	redirect_heredoc_2(&heredoc_file);
	dup2(saved_stdin, STDIN_FILENO);
	while (1)
	{
		input = readline("> ");
		if (!input || ft_strcmp(input, delimiter) == 0)
		{
			free(input);
			break ;
		}
		input = expa_chang(input, all);
		input_len = ft_strlen(input);
		if (write(heredoc_file, input, input_len) == -1 || \
		write(heredoc_file, "\n", 1) == -1)
			redirect_heredoc_3(all);
		free(input);
	}
	redirect_input("heredoc_temp_file.txt", all);
	close(heredoc_file);
}
