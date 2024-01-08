/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 00:52:02 by myassine          #+#    #+#             */
/*   Updated: 2024/01/08 21:10:39 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*removeCharAtIndex(char *str, int i)
{
    int	j;
    int	k;

	j = 0;
	k = 0;
    if (str == NULL || i < 0)
        return (NULL);
    char *newStr = (char *)ft_calloc(ft_strlen(str), 1);
    if (newStr == NULL)
        return (NULL);
    while (str[k] != '\0')
	{
        if (k != i)
            newStr[j++] = str[k];
        k++;
    }
    return (newStr);
}

void redirect_heredoc_2(int *heredoc_file)
{
	(*heredoc_file) = open("heredoc_temp_file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if ((*heredoc_file) == -1)
	{
    	perror("Erreur lors de la création du fichier heredoc");
    	exit(EXIT_FAILURE);
    }
}

void redirect_heredoc_3()
{
	perror("Erreur lors de l'écriture dans le fichier heredoc");
    exit(EXIT_FAILURE);
}

void redirect_heredoc(char *delimiter, t_env *env, t_env *head_env, int saved_stdin)
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
            break;
        }
        input = expa_chang(input, env, head_env);
        input_len = ft_strlen(input);
        if (write(heredoc_file, input, input_len) == -1 || write(heredoc_file, "\n", 1) == -1)
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
	if(test->arg)
	{
		while(test->arg[(*j_a)])
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

int	main(int argc, char **argv, char *envp[])
{
	if(argc > 1)
		return (FAILURE);
	pid_t	pid;
	t_env	*env;
	t_env	*head_env;
	t_block *test;
	char	*input;
	char	*path;
	char	*str;
	char	*tmp_path;
	char	**envs;
	char	**args;
	int		pipe_fds[2];
	int		prev_pipe_fd;
	int		saved_stdout;
	int		saved_stdin;
	int		command_alone;
	int		i_a;
	int		j_a;
	int		status;

	env = NULL;
	envs = NULL;
	head_env = NULL;
	env = init_env(env, head_env, envp);
	pid = -1;
	path = NULL;
	while(1)
	{
		signal(SIGINT, &sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		envs = ft_lstsplit(&env);
		input = NULL;
		path = get_current_directory_with_prompt();
		if (!path)
			return (FAILURE);
		i_a = 0;
		while (path[i_a])
			i_a++;
		if (path[--i_a] != ' ')
			tmp_path = ft_strjoin(path, "$> ");
		path = tmp_path;
		if (!path)
			return (0);
		input = readline(path);
		if (!input)
		{
			eof(input, envs, env, head_env);
			exit(0);
		}
		add_history(input);
		if (!no_input(input))
		{
			if (input)
				free(input);
			continue;
		}
		if (check_error(input))
		{
			print_error(check_error(input));
			continue;
		}
		i_a = 0;
		command_alone = 0;
		while (input && input[i_a])
		{
			if (input[i_a] == '|')
				command_alone++;
			i_a++;
		}
		in_quote(input);
		input = expa_chang(input, env, head_env);
		test = NULL;
		if(check_error(input))
		{
			print_error(check_error(input));
			continue;
		}	
		test = tokenization(input);
		saved_stdout = dup(STDOUT_FILENO);
    	saved_stdin = dup(STDIN_FILENO);
		if (command_alone > 0)
		{
			printf(BACK_RED"multi"RST"\n");
			prev_pipe_fd = -1;
			while (test)
			{
				return_neg(test->cmd);
				test->cmd = if_quote(test->cmd);
				if (!test->arg)
					args = malloc(sizeof(char *) * 2);
				else
					args = malloc(sizeof(char *) * (len_tab(test->arg) + 2));
				if(!args)
					return (0);
				i_a = 1;
				j_a = 0;
				args[0] = ft_strdup(test->cmd);
				args[i_a] = NULL;
				for_arg(test, &i_a, &j_a, args);
				while(args[i_a])
				{
					return_neg(args[i_a]);
					i_a++;
				}
				str = NULL;
				if (args[0] == NULL)
					;
				else if((str = verif_cmd(args, envs)) == NULL)
					{
					}
				if(str)
				{
					args[0] = ft_strdup(str);
					free(str);
				}
				if (pipe(pipe_fds) == -1)
					exit(EXIT_FAILURE);
				if ((pid = fork()) < 0) 
				{
					printf("fork\n");
					exit(EXIT_FAILURE);
				}
				if (pid == 0)
				{
					if (prev_pipe_fd != -1)
					{
						if (dup2(prev_pipe_fd, STDIN_FILENO) == -1)
							exit(EXIT_FAILURE);
						close(prev_pipe_fd);
					}
					if (test->next)
					{
						if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)
							exit(EXIT_FAILURE);
						close(pipe_fds[1]);
					}
					apply_redirections_to_command_line(test, env, head_env, saved_stdin);
					if(args && !ft_strcmp(test->cmd, "exit" ))
					{
						if(ft_exit_1(test))
						{
							eof(input, envs, env, head_env);
							exit(ft_atoi(test->arg[0]));
						}
					}
					if(applic_bulltin(test, env, head_env, args))
						exit(0);
					else if(execve(args[0], args, envs) == -1)
					{
						if(test->cmd)
							printf("%s: command not found\n", test->cmd);
						all_free_1(test, env, head_env, args);
						exit(127);
					}
				}
				else
				{
 					if (test->next)
					{
						close(pipe_fds[1]);
						prev_pipe_fd = pipe_fds[0];
					}
					else
					{
						close(pipe_fds[0]);
						close(pipe_fds[1]);					
					}
					test = test->next;
				}		
			}
		}
		else
		{
			printf(BACK_GREEN"alone"RST"\n");
			return_neg(test->cmd);
			test->cmd = if_quote(test->cmd);
			if (!test->arg)
				args = malloc(sizeof(char *) * 2);
			else
				args = malloc(sizeof(char *) * (len_tab(test->arg) + 2));
			if(!args)
				return (0);
			i_a = 1;
			j_a = 0;
			args[0] = ft_strdup(test->cmd);
			args[i_a] = NULL;
			for_arg(test, &i_a, &j_a, args);
			while(args[i_a++])
				return_neg(args[i_a]);
			str = NULL;
			apply_redirections_to_command_line(test, env, head_env, saved_stdin);
			if (args[0] == NULL)
				;
			else if((str = verif_cmd(args, envs)) == NULL)
				{
				}
			if(str)
			{
				args[0] = ft_strdup(str);
				free(str);
			}
			if(!ft_strcmp(test->cmd, "exit" ))
				if(ft_exit_1(test))
				{
					if(!test->arg)
					{
						eof(input, envs, env, head_env);
						exit(0);
					}
					eof(input, envs, env, head_env);
					exit(ft_atoi(test->arg[0]));
				}
			if(applic_bulltin(test, env, head_env, args))
				;
			else if ((pid = fork()) < 0) 
			{
				printf("fork\n");
				exit(EXIT_FAILURE);
			}
			else if (pid == 0 && !is_bultin(args[0]) && execve(args[0], args, envs) == -1)
			{
				if(args[0] && test->cmd)
					printf("%s: command not found\n", args[0]);
				all_free_1(test, env, head_env, args);
				exit(127);
			}
			if(envs)
				free_string_array(envs);
		}
		unlink("heredoc_temp_file.txt");
		dup2(saved_stdin ,STDIN_FILENO);
		dup2(saved_stdout ,STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		signal(SIGINT, SIG_IGN);
		while (1)
		{
			pid = waitpid(-1, &status, /*WNOHANG*/0);
			if (pid < 0)
				break ;
		}
		if (args)
			free_string_array(args);
	}
	terminat(input, envs, env, head_env);
	i_a = -1;
	while (args[++i_a])
	{
		free(args[i_a]);
		args[i_a] = NULL;
	}
	free(args);
	all_free_1(test, env, head_env, args);
	argv = argv;
	return (SUCCESS);
}

/*
	A FAIRE ;
		leaks a gerer
		$? valeur de retour a gerer
		norm

*/