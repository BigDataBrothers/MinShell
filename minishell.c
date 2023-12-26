/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamil <mgamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 00:52:02 by myassine          #+#    #+#             */
/*   Updated: 2023/12/26 18:51:06 by mgamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	**ft_lstsplit(t_env **lst)
// {
// 	char	**res;
// 	t_env	*ptr;
// 	int		size;
// 	int		i;
// 	int		h;
// 	char *tmp;

// 	ptr = *lst;
// 	i = -1;
// 	size = ft_lstsize(ptr);
// 	res = (char **)ft_calloc(sizeof(char *) * (size + 1));
// 	if (!res)
// 		return (NULL);
// 	while (++i < size)
// 	{
// 		if(!ft_strcmp("SHLVL", ptr->str[0]))
// 		{
// 			h = ft_atoi(ptr->str[1])++;
// 			tmp = ft_strjoin(ptr->str[0], "=");
// 			char *itoa = ft_itoa(h);
// 			if (!itoa)
// 				return NULL;
// 			res[i] = ft_strjoin(tmp, itoa);
// 			free(itoa);
// 			if (tmp)
// 			free(tmp);
// 			if (!res[i])
// 				return (NULL);
// 			ptr = ptr->next;
// 			continue;
// 		}
// 		tmp = ft_strjoin(ptr->str[0], "=");
// 		res[i] = ft_strjoin(tmp, ptr->str[1]);
// 		if (tmp)
// 			free(tmp);
// 		if (!res[i])
// 			return (NULL);
// 		ptr = ptr->next;
// 	}
// 	return (res);
// }




char *removeCharAtIndex(char *str, int i)
{
    int j;
    int k;
	
	j = 0;
	k = 0;
    if (str == NULL || i < 0)
        return NULL;
    char *newStr = (char *)ft_calloc(ft_strlen(str), 1);
    if (newStr == NULL)
        return NULL;
    while (str[k] != '\0')
	{
        if (k != i)
            newStr[j++] = str[k];
        k++;
    }
    return (newStr);
}


//ls -l | grep .c | wc -l > file.txt < input.txt

void printBlock(t_block *block)
{
    if (block == NULL)
	{
        printf("Block is NULL\n");
        return;
    }
    if (block->arg != NULL)
	{
        printf("Arguments/Options:\n");
        for (int i = 0; block->arg[i] != NULL; ++i)
            printf("%s\n", block->arg[i]);
    }
	else
        printf("No arguments/options\n");
	if(block->dir)
	{
		while (block->dir)
		{
			dprintf(2, "[%d]\n", block->dir->fd);
			dprintf(2, "[%d]\n", block->dir->type);
			dprintf(2, "[%s]\n", block->dir->file);
			block->dir = block->dir->next;
		}
	}
}

//Un pipe est egal a une redirection in et out


void redirect_heredoc(char *delimiter, t_env *env, t_env *head_env, int saved_stdin)
{
    int heredoc_file = open("heredoc_temp_file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (heredoc_file == -1) {
        perror("Erreur lors de la création du fichier heredoc");
        exit(EXIT_FAILURE);
    }
    char *input;
    size_t input_len;
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
		{
            perror("Erreur lors de l'écriture dans le fichier heredoc");
            exit(EXIT_FAILURE);
        }
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
*/

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
	char	**envs;
	char	**args;

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
		if(!path)
			return (FAILURE);
		int u = 0;
		while(path[u])
			u++;
		char *tmp_path;
		if(path[--u] != ' ')
			tmp_path = ft_strjoin(path, "$> ");
		path = tmp_path;
		if(!path)
			return (0);
		input = readline(path);
		if (!input)
			eof(input, envs, env, head_env, 0);
		add_history(input);
		if(!no_input(input))
		{
			if(input)
				free(input);
			continue;
		}
		if(check_error(input))
		{
			print_error(check_error(input));
			continue;
		}
		int	i_alone = 0;
		int command_alone = 0;
		while (input && input[i_alone])
		{
			if (input[i_alone] == '|')
				command_alone++;
			i_alone++;
		}
		in_quote(input);
		input = expa_chang(input, env, head_env);
		test = NULL;
		test = tokenization(input);//chaque t_block et 1 cmd + args
		(void)pid;
		int saved_stdout = dup(STDOUT_FILENO);
    	int saved_stdin = dup(STDIN_FILENO);
		if (command_alone > 0)//Gere les pipeline ICI
		{
			printf(BACK_RED"multi"RST"\n");
			int pipe_fds[2];
			int prev_pipe_fd = -1;
			while(test)
			{
				return_neg(test->cmd);
				test->cmd = if_quote(test->cmd);
				if (!test->arg)
					args = malloc(sizeof(char *) * 2);
				else
					args = malloc(sizeof(char *) * (len_tab(test->arg) + 2));
				if(!args)
					return (0);
				int i = 1;
				int j = 0;

				args[0] = ft_strdup(test->cmd);
				args[i] = NULL;
				if(test->arg)
				{
					while(test->arg[j])
					{
						return_neg(test->arg[j]);
						test->arg[j] = if_quote(test->arg[j]);
						args[i] = ft_strdup(test->arg[j]);
						i++;
						j++;
					}
					args[i] = NULL;
				}
				int a = 0;
				while(args[a])
				{
					return_neg(args[a]);
					a++;
				}
				char *str;
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
				// [START OF CHILDREN'S JOURNEY TO EXEC]
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
							eof(input, envs, env, head_env, ft_atoi(test->arg[0]));
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
			int i = 1;
			int j = 0;
			args[0] = ft_strdup(test->cmd);
			args[i] = NULL;
			if(test->arg)
			{
				while(test->arg[j])
				{
					return_neg(test->arg[j]);
					test->arg[j] = if_quote(test->arg[j]);
					args[i] = ft_strdup(test->arg[j]);
					i++;
					j++;
				}
				args[i] = NULL;
			}
			int a = 0;
			while(args[a])
			{
				return_neg(args[a]);
				a++;
			}
			char *str;
			str = NULL;
			apply_redirections_to_command_line(test, env, head_env, saved_stdin);
			if (args[0] == NULL)
				{}
			else if((str = verif_cmd(args, envs)) == NULL)
				{
				}
			if(str)
			{
				args[0] = ft_strdup(str);
				free(str);
			}
			if(!ft_strcmp(test->cmd, "exit" ))
			{
				if(ft_exit_1(test))
				{
					if(!test->arg)
						eof(input, envs, env, head_env, 0);
					eof(input, envs, env, head_env, (char)ft_atoi(test->arg[0]));
				}
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
				freeStringArray(envs);
		}
		unlink("heredoc_temp_file.txt");
		dup2(saved_stdin ,STDIN_FILENO);
		dup2(saved_stdout ,STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		signal(SIGINT, SIG_IGN);
		int status;
		while (1)
		{
			pid = waitpid(-1, &status, WNOHANG);
			if (pid < 0)
				break;
		}
		if(test)
			free_block_list(test);
		if(args)
			freeStringArray(args);
	}
	terminat(input, envs, env, head_env);
	int i_free_args = -1;
	while (args[++i_free_args])
	{
		free(args[i_free_args]);
		args[i_free_args] = NULL;
	}
	free(args);
	all_free_1(test, env, head_env, args);
	argc = argc;
	argv = argv;
	return (SUCCESS);
}


/*
	A FAIRE ;
		leaks a gerer
		$? valeur de retour a gerer
		norm

*/