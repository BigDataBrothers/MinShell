/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 00:52:02 by myassine          #+#    #+#             */
/*   Updated: 2023/12/21 23:22:26 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
/mnt/nfs/homes/myassine/Desktop/gv/MinShell$> << a
> a
exit        why print exit?

/mnt/nfs/homes/myassine/Desktop/gv/MinShell$> echo $?
$?

/mnt/nfs/homes/myassine/Desktop/gv/MinShell$> unset PATH
/mnt/nfs/homes/myassine/Desktop/gv/MinShell$> export PATH=/usr/bin
/mnt/nfs/homes/myassine/Desktop/gv/MinShell$> ls
ls: command not found
*/


void	eof(char *input, char **envs, t_env *env, t_env *head_env)
{
	// printf("&exit&\n");
	terminat(input, envs, env, head_env);
	exit(0);
}

char *get_current_directory_with_prompt()
{
	static char	cwd[PATH_MAX];

	getcwd(cwd, sizeof(cwd));
	return (cwd);
}
//ls -l | grep .c | wc -l > file.txt < input.txt

void fait_rien()
{
	return ;
}

char	**ft_lstsplit(t_env **lst)
{
	char	**res;
	t_env	*ptr;
	int		size;
	int		i;
	int		h;
	char *tmp;

	ptr = *lst;
	i = -1;
	size = ft_lstsize(ptr);
	res = (char **)malloc(sizeof(char *) * (size + 1));
	if (!res)
		return (NULL);
	while (++i < size)
	{
		if(!ft_strcmp("SHLVL", ptr->str[0]))
		{
			h = ft_atoi(ptr->str[1]);
			h++;
			tmp = ft_strjoin(ptr->str[0], "=");
			char *itoa = ft_itoa(h);
			if(!itoa)
				return NULL;
			res[i] = ft_strjoin(tmp, itoa);
			free(itoa);
			if (tmp)
			free(tmp);
			if (!res[i])
				return (NULL);
			ptr = ptr->next;
			continue;
		}
		tmp = ft_strjoin(ptr->str[0], "=");
		res[i] = ft_strjoin(tmp, ptr->str[1]);
		if (tmp)
			free(tmp);
		if (!res[i])
			return (NULL);
		ptr = ptr->next;
	}
	res[i] = NULL;
	return (res);
}


char **getPath(char **env)
{
	int i = 0;
	char **path = NULL;
	while(env[i])
	{
		if(ft_strstr(env[i], "PATH=/mnt/nfs/homes/myassine/bin:"))	
			{
				path = ft_split_path(env[i], ':');
				break;
			}
		i++;
	}
	return (path);
}

char	*verif_cmd(char **args, char **env)
{
	if(args[0] == NULL)
	 	return (NULL);
	if(is_bultin(args[0]))
		return (args[0]);
	if(access(args[0], F_OK) == 0)
		return (args[0]);
	char *tmp = NULL;
	char **path = NULL;
	path = getPath(env);
	if(path && path[0])
	{
		tmp = ft_substr(path[0], 5, ft_strlen(path[0]));
		free(path[0]);
		path[0] = strdup(tmp);
		free(tmp);
	}
	int i = 0;
	char *str = NULL;
	tmp = ft_strdup(args[0]);
	free(args[0]);
	args[0] = ft_strjoin("/", tmp);
	free(tmp);
	if(path)
	{
		while(path[i])
		{
			str = ft_strjoin(path[i], args[0]);
			if(access(str, F_OK) == 0)
			{
				freeStringArray(path);
				free(args[0]);
				return (str);
			}
			free(str);
			str = NULL;
			i++;
		}
	}
	freeStringArray(path);	
	tmp = ft_substr(args[0], 1, ft_strlen(args[0])); 
	free(args[0]);
	args[0] = NULL;
	args[0] = ft_strdup(tmp);
	if (tmp)
		free(tmp);
	return (NULL);
}

int	no_input(char *input)
{
	int i = 0;
	if(!input || input[i] == '\0')
		return (0);
	skip_whitespace(input, &i);
	if(!input || input[i] == '\0')
		return (0);
	return (1);
}


char *removeCharAtIndex(char *str, int i)
{
    if (str == NULL || i < 0)
        return NULL;
    char *newStr = (char *)malloc(ft_strlen(str));
    if (newStr == NULL)
        return NULL;
    int j = 0;
    int k = 0;
    while (str[k] != '\0')
	{
        if (k != i)
		{
            newStr[j] = str[k];
            j++;
        }
        k++;
    }
    newStr[j] = '\0';
    return (newStr);
}

void handleCtrlC()
{
    write(STDOUT_FILENO, "\n", 1);
}

void    sigint_handler(int sig)
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

//ls -l | grep .c | wc -l > file.txt < input.txt

void all_free_1(t_block *test, char *input)
{
	if(test)
		free_block_list(test);
	if(input)
		free(input);
}

void printBlock(t_block *block)
{
    if (block == NULL)
	{
        printf("Block is NULL\n");
        return;
    }

    printf("Command: %s\n", block->cmd);

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

//Un pipe est egal a une redirection in et out


void redirect_output(char *filename)
{
    int file = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);

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

void append_output(char *filename)
{
    int file = open(filename, O_CREAT | O_APPEND | O_WRONLY, 0644);

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

void redirect_input(char *filename)
{
    int file = open(filename, O_RDONLY);

    if (file == -1) 
	{
        printf("minishell: %s: No such file or directory\n", filename);
		exit(EXIT_FAILURE);
    }

    dup2(file, STDIN_FILENO);
    close(file);
}


void redirect_heredoc(char *delimiter, t_env *env, t_env *head_env, int saved_stdin)
{
    char *filename = "heredoc_temp_file.txt";
    int heredoc_file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (heredoc_file == -1) {
        perror("Erreur lors de la création du fichier heredoc");
        exit(EXIT_FAILURE);
    }
    char *input;
    // size_t delimiter_len = ft_strlen(delimiter);
    size_t input_len;
	dup2(saved_stdin, STDIN_FILENO);
	while (1)
	{
		char *prompt = "> ";
        input = readline(prompt);
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

void apply_redirections_to_command_line(t_block *test, t_env *env, t_env *head_env, int saved_stdin)
{	
	while (test && test->dir && test->dir->file)
	{
		if (test->dir->type == APPEND)
			append_output(test->dir->file);
		else if (test->dir->type ==  IN)
			redirect_input(test->dir->file);
		else if (test->dir->type ==  OUT)
			redirect_output(test->dir->file);
		else if(test->dir->type == HEREDOC)
			redirect_heredoc(test->dir->file, env, head_env, saved_stdin);
		test->dir = test->dir->next;
	}
}
/*
	consider les bulltin comme des command pour ne pas print que la cmd existe pas
	gerer tout les cas avec des quot char negatif
	si command ecrit avec chemin absolut l executer
*/

int is_bultin(char *args)
{
	if(!ft_strcmp(args, "cd" ))// && chdir(args[1]) == 0)
		return (1);
	// else if(!ft_strcmp(args[0], "cd" ) && !args[1] && chdir(ft_get_env("HOME", env, head_env)) == 0)
		// return (1);
	else if(!ft_strcmp(args, "exit"))
		return (1);
	else if(!ft_strcmp(args, "pwd"))
		return (1);
	else if(!ft_strcmp(args, "echo"))
		return (1);
	else if(!ft_strcmp(args, "unset"))
		return (1);
	else if(!ft_strcmp(args, "env"))
		return (1);
	else if(!ft_strcmp(args, "export"))
		return (1);
	return (0);
}

void execute_pipeline(t_block *pipeline) {
    t_block *current_block = pipeline;
    int prev_pipe[2];
    int new_pipe[2];

    while (current_block != NULL) {
        // Create a new pipe for each command (except the last one)
        if (current_block->next != NULL) {
            if (pipe(new_pipe) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // Child process
            // Set up redirection for input
            if (current_block != pipeline) {
                dup2(prev_pipe[0], STDIN_FILENO);
                close(prev_pipe[0]);
                close(prev_pipe[1]);
            }

            // Set up redirection for output
            if (current_block->next != NULL) {
                close(new_pipe[0]);
                dup2(new_pipe[1], STDOUT_FILENO);
                close(new_pipe[1]);
            }

            // Execute the command
            execvp(current_block->cmd, current_block->arg);

            // If execvp fails
            perror("execvp");
            exit(EXIT_FAILURE);
        } else { // Parent process
            // Close previous pipe (no longer needed in the parent)
            if (current_block != pipeline) {
                close(prev_pipe[0]);
                close(prev_pipe[1]);
            }

            // Save the current pipe for the next iteration
            if (current_block->next != NULL) {
                prev_pipe[0] = new_pipe[0];
                prev_pipe[1] = new_pipe[1];
            }

            current_block = current_block->next;
        }
    }

    // Wait for all child processes to finish
    while (wait(NULL) > 0);
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
	char	**envs;
	char	**args;
	int		error;

	env = NULL;
	envs = NULL;
	head_env = NULL;
	env = init_env(env, head_env, envp);
	error = 0;
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
		if(path[--u] != ' ')
			strcat(path, "$> ");//recoder
		if(!path)
			return (0);
		input = readline(path);
		if (!input)
			eof(input, envs, env, head_env);
		add_history(input);
		if(!no_input(input))
		{
			if(input)
				free(input);
			continue;
		}
		error = check_error(input);
		if(error)
		{
			print_error(error);
			continue;
		}
		
		// ------------------------ start gael -------------------------
		int	i_alone = 0;
		int command_alone = 0;
		while (input[i_alone])
		{
			if (input[i_alone] == '|')
				command_alone++;
			i_alone++;
		}
		// ------------------------ end gael -------------------------
		
		
		in_quote(input);
		input = expa_chang(input, env, head_env);
		test = NULL;
		test = tokenization(input);//chaque t_block et 1 cmd + args
		return_neg(test->cmd);
		test->cmd = if_quote(test->cmd);

			
		if (!test->arg)
			args = malloc(sizeof(char *) * 2);
		else
			args = malloc(sizeof(char *) * (len_tab(test->arg) + 2));
		if(!args)
			return (0);
		int i = 0;
		int j = 0;

		args[i] = ft_strdup(test->cmd);
		i++;
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

		// --------------------- start gael ----------------------
		(void)pid;
		int saved_stdout = dup(STDOUT_FILENO);
    	int saved_stdin = dup(STDIN_FILENO);
		if (command_alone > 0)
		{
			printf(BACK_RED"multi"RST"\n");
			// [START OF CHILDREN'S JOURNEY TO EXEC]
			while(test)//Gere les pipeline ICI
			{
				char *str;
				str = NULL;
				apply_redirections_to_command_line(test, env, head_env, saved_stdin);
				if (args[0] == NULL)
					;
				else if((str = verif_cmd(args, envs)) == NULL)
					{}
				if(str)
				{
					args[0] = ft_strdup(str);
					free(str);
				}
				if(args && !ft_strcmp(test->cmd, "exit" ))
				{
					dprintf(2, "exit\n");
					eof(input, envs, env, head_env);
				}
				else if(args && !ft_strcmp(test->cmd, "cd" ) && chdir(args[1]) == 0)
					;
				else if(args && !ft_strcmp(test->cmd, "cd" ) && !args[1] && chdir(ft_get_env("HOME", env, head_env)) == 0)
					;
				else if(args && !ft_strcmp(test->cmd, "unset"))
					ft_unset(env, head_env, input);
				else if(args && !ft_strcmp(test->cmd, "export"))
					ft_export(env, head_env, envs, input);
				else if(args && !ft_strcmp(test->cmd, "pwd"))
					ft_pwd(input);
				else if(args && !ft_strcmp(test->cmd, "echo"))
					ft_echo(test);
				else if(args && !ft_strcmp(test->cmd, "env"))
					ft_env(env, head_env, input);
				else if ((pid = fork()) < 0) 
				{
					printf("fork\n");
					exit(EXIT_FAILURE);
				}
				if (pid == 0)
				{
					if(execve(args[0], args, envs) == -1)
					{
						if(test->cmd)
							printf("%s: command not found\n", test->cmd);
						if(envs)
							freeStringArray(envs);
						if(args)
							freeStringArray(args);
						free(args);
						all_free_1(test, input);
						if(env)
							free_env(env, head_env);
						exit(127);
					}
					if(envs)
						freeStringArray(envs);
					// [END OF CHILDREN'S JOURNEY TO EXEC]
				}
				test = test->next;
			}
		}
		else
		{
			printf(BACK_GREEN"alone"RST"\n");
			char *str;
			str = NULL;
			apply_redirections_to_command_line(test, env, head_env, saved_stdin);
			if (args[0] == NULL)
				{}
			else if((str = verif_cmd(args, envs)) == NULL)
				{}
			if(str)
			{
				args[0] = ft_strdup(str);
				free(str);
			}
			if(!ft_strcmp(args[0], "exit" ))
			{
				dprintf(2, "@exit\n");
				eof(input, envs, env, head_env);
			}
			else if(!ft_strcmp(args[0], "cd" ) && chdir(args[1]) == 0)
				;
			else if(!ft_strcmp(args[0], "cd" ) && !args[1] && chdir(ft_get_env("HOME", env, head_env)) == 0)
				;
			else if(!ft_strcmp(args[0], "unset"))
				ft_unset(env, head_env, input);
			else if(!ft_strcmp(args[0], "export"))
				ft_export(env, head_env, envs, input);
			else if(!ft_strcmp(args[0], "pwd"))
				ft_pwd(input);
			else if(!ft_strcmp(args[0], "echo"))
				ft_echo(test);
			else if(!ft_strcmp(args[0], "env"))
				ft_env(env, head_env, input);
			else if ((pid = fork()) < 0) 
			{
				printf("fork\n");
				exit(EXIT_FAILURE);
			}
			if (pid == 0 && !is_bultin(args[0]) && execve(args[0], args, envs) == -1)
			{
				if(args[0] && test->cmd)
					printf("%s: command not found\n", args[0]);
				if(envs)
					freeStringArray(envs);
				if(args)
					freeStringArray(args);
				all_free_1(test, input);
				if(env)
					free_env(env, head_env);
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
		// ---------------- end gael --------------------

		signal(SIGINT, SIG_IGN);
		int status;	
		waitpid(-1, &status, 0);
		if(test)
			free_block_list(test);
		free(input);
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
	all_free_1(test, input);
	argc = argc;
	argv = argv;
	envp = envp;
	return (SUCCESS);
}


/*
	A FAIRE ;
		pipe a gerer
			; 
				rediriger a l aide des nvx int de t_token;
				savoir ou enlever les free;
		savoir p je doit quit 2x
		leaks a gerer
		$? valeur de retour a gerer
		verifier si command interdite
		norm(appeler Gael)

*/