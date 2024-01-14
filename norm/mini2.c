/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:07:13 by myassine          #+#    #+#             */
/*   Updated: 2024/01/14 18:08:47 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(2, "\n", 1);
		rl_on_new_line();
		rl_clear_history();
		rl_replace_line("", 0);
		rl_redisplay();
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

void	redirect_output(char *filename)
{
	int	file;

	file = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (file == -1)
	{
		perror("Erreur lors de l'ouverture du fichier");
		exit(EXIT_FAILURE);
	}
	if (dup2(file, STDOUT_FILENO) == -1)
	{
		perror("Erreur lors de la redirection de la sortie standard");
		close(file);
		exit(EXIT_FAILURE);
	}
	close(file);
}

void	append_output(char *filename)
{
	int	file;

	file = open(filename, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (file == -1)
	{
		perror("Erreur lors de l'ouverture du fichier en mode ajout");
		exit(EXIT_FAILURE);
	}
	if (dup2(file, STDOUT_FILENO) == -1)
	{
		perror("Erreur lors de la redirection de la sortie standard");
		close(file);
		exit(EXIT_FAILURE);
	}
	close(file);
}
