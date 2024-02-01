/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 17:47:27 by myassine          #+#    #+#             */
/*   Updated: 2024/02/01 20:12:32 by myassine         ###   ########.fr       */
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
		input = expa_chang(input, all->env, all->head_env);
		input_len = ft_strlen(input);
		if(all->test->dir->next && all->test->dir->next->type == OUT)
		if (write(heredoc_file, input, input_len) == -1 || \
		write(heredoc_file, "\n", 1) == -1)
			redirect_heredoc_3(all);
		free(input);
	}
	redirect_input("heredoc_temp_file.txt");
	// if (all->test->dir->next && all->test->dir->next->type == OUT)
        // dup2(heredoc_file, STDOUT_FILENO);
    close(heredoc_file);
	// close(heredoc_file);
}


	
	// close(heredoc_file);  // Ferme heredoc_file après la boucle
    // redirect_input("heredoc_temp_file.txt");  // Rétablit la redirection standard d'entrée
    // if (all->test->dir->next && all->test->dir->next->type == OUT)
    //     dup2(STDOUT_FILENO, heredoc_file);
	//printf("alll -==== %s\n", all->test->dir);
	// if (all->test->dir->next->fd)
	// 	redirect_output_heredoc("heredoc_temp_file.txt", all->test->dir->next->fd);