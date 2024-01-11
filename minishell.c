/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 00:52:02 by myassine          #+#    #+#             */
/*   Updated: 2024/01/11 19:18:44 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_char_at_index(char *str, int i)
{
	char	*new_str;
	int		j;
	int		k;

	j = 0;
	k = 0;
	new_str = (char *)ft_calloc(ft_strlen(str), 1);
	if (str == NULL || i < 0)
		return (NULL);
	if (new_str == NULL)
		return (NULL);
	while (str[k] != '\0')
	{
		if (k != i)
			new_str[j++] = str[k];
		k++;
	}
	return (new_str);
}

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

void	redirect_heredoc_3(void)
{
	perror("Erreur lors de l'écriture dans le fichier heredoc");
	exit(EXIT_FAILURE);
}

void	redirect_heredoc(char *delimiter, t_env *env, \
t_env *head_env, int saved_stdin)
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
		input = expa_chang(input, env, head_env);
		input_len = ft_strlen(input);
		if (write(heredoc_file, input, input_len) == -1 || \
		write(heredoc_file, "\n", 1) == -1)
			redirect_heredoc_3();
		free(input);
	}
	redirect_input("heredoc_temp_file.txt");
	close(heredoc_file);
}

/*
	consider les bulltin comme des command pour ne pas print que la cmd existe pas
	gerer tout les cas avec des quot char negatif
	si command ecrit avec chemin absolut l executer
*/


/*myassine@made-f0Br1s4:~/MIN/MIN$ sdggdf | ls
builtins.c	check_redir_a_pipe.c  lib_mini.c   print.c
builtins.o	check_redir_a_pipe.o  lib_mini.o   print.o
check_char_2.c	env.c		      Makefile	   skip_char.c
check_char_2.o	env.o		      malloc.c	   skip_char.o
check_char.c	expand.c	      malloc.o	   tokenization.c
check_char.o	expand.o	      minishell    tokenization.o
check_error.c	free.c		      minishell.c  valgrind_BU.log
check_error.o	free.o		      minishell.h  valgrind_leaks_ignore.txt
check_quote.c	ft_split_path.c       minishell.o  valgrind.log
check_quote.o	ft_split_path.o       output.txt
sdggdf: command not found


export a
*/

void	for_arg(t_block *test, int *i_a, int *j_a, char **args)
{
	if (test->arg)
	{
		while (test->arg[(*j_a)])
		{
			return_neg(test->arg[(*j_a)]);
			test->arg[(*j_a)] = if_quote(test->arg[(*j_a)]);
			args[(*i_a)] = ft_strdup(test->arg[(*j_a)]);
			(*i_a)++;
			(*j_a)++;
		}
		args[(*i_a)] = NULL;
	}
	(*i_a) = 0;
}

char	**creat_args(t_block *test, int *i_a, int *j_a)
{
	char	**args;

	return_neg(test->cmd);
	test->cmd = if_quote(test->cmd);
	if (!test->arg)
		args = malloc(sizeof(char *) * 2);
	else
		args = malloc(sizeof(char *) * (len_tab(test->arg) + 2));
	if (!args)
		return (NULL);
	(*i_a) = 1;
	(*j_a) = 0;
	args[0] = ft_strdup(test->cmd);
	args[(*i_a)] = NULL;
	for_arg(test, &(*i_a), &(*j_a), args);
	while (args[(*i_a)++])
		return_neg(args[(*i_a)]);
	return (args);
}

void	free_tab(char **args)
{
	int	i_a;

	i_a = -1;
	while (args[++i_a])
	{
		free(args[i_a]);
		args[i_a] = NULL;
	}
}

int	main(int argc, char **argv, char *envp[])
{
	t_all	all;

	init_all(&all, envp);
	if (argc > 1)
		return (FAILURE);
	while (1)
	{
		if (start_input(&all) == FAILURE)
			return (FAILURE);
		check_error_input(&all);
		parsing(&all);
		if (all.command_alone > 0)
		{
			all.prev_pipe_fd = -1;
			while (all.test)
			{
				all.args = creat_args(all.test, &all.i_a, &all.j_a);
				all.str = verif_cmd(all.args, all.envs);
				if (all.str != NULL)
				{
					all.args[0] = ft_strdup(all.str);
					free(all.str);
				}
				if (pipe(all.pipe_fds) == -1)
					exit(EXIT_FAILURE);
				all.pid = fork();
				if (all.pid < 0)
				{
					perror("fork\n");
					exit(EXIT_FAILURE);
				}
				if (all.pid == 0)
				{
					dup_in_child(&all);
					if (applic_bulltin(all.test, all.env, all.head_env, all.args))
						exit(0);
					else if (execve(all.args[0], all.args, all.envs) == -1)
					{
						if (all.test->cmd)
							printf("%s: command not found\n", all.test->cmd);
						all_free_1(all.test, all.env, all.head_env, all.args);
						exit(127);
					}
				}
				else
				{
					if (all.test->next)
					{
						close(all.pipe_fds[1]);
						all.prev_pipe_fd = all.pipe_fds[0];
					}
					else
					{
						close(all.pipe_fds[0]);
						close(all.pipe_fds[1]);
					}
					all.test = all.test->next;
				}
			}
		}
		else
		{
			all.args = creat_args(all.test, &all.i_a, &all.j_a);
			apply_redirections_to_command_line(all.test, all.env, \
			all.head_env, all.saved_stdin);
			all.str = verif_cmd(all.args, all.envs);
			if (all.str != NULL)
				all.args[0] = ft_strdupf(all.str);
			if (!ft_strcmp(all.test->cmd, "exit" ))
			{
				if (ft_exit_1(all.test))
				{
					if (!all.test->arg)
					{
						eof(all.input, all.envs, all.env, all.head_env);
						exit(0);
					}
					eof(all.input, all.envs, all.env, all.head_env);
					exit(ft_atoi(all.test->arg[0]));
				}
			}
			if (!applic_bulltin(all.test, all.env, all.head_env, all.args))
			{
				all.pid = fork();
				if ((all.pid) < 0)
				{
					perror("fork\n");
					exit(EXIT_FAILURE);
				}
				else if (all.pid == 0 && !is_bultin(all.args[0]) && \
				execve(all.args[0], all.args, all.envs) == -1)
				{
					if (all.args[0] && all.test->cmd)
						printf("%s: command not found\n", all.args[0]);
					all_free_1(all.test, all.env, all.head_env, all.args);
					exit(127);
				}
			}
			free_string_array(all.envs);
		}
		end_prompt(&all);
	}
	terminat(all.input, all.envs, all.env, all.head_env);
	argv = argv;
	return (all_free_1(all.test, all.env, all.head_env, all.args), SUCCESS);
}

/*
	A FAIRE ;
		leaks a gerer
		$? valeur de retour a gerer
		norm
		chercher dans sujets fonctions autorisees    -> isatty   avant argc

*/