/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 17:41:49 by myassine          #+#    #+#             */
/*   Updated: 2024/01/18 20:11:03 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	prepare_block(t_all *all)
{
	all->args = creat_args(all->test, &all->i_a, &all->j_a);
	all->str = verif_cmd(all->args, all->envs);
	if (all->str != NULL)
	{
		all->args[0] = ft_strdup(all->str);
		free(all->str);
	}
	if (pipe(all->pipe_fds) == -1)
	{
		all_free_1(all->test, all->env, all->head_env, all->args);
		exit(EXIT_FAILURE);
	}
	all->pid = fork();
	if (all->pid < 0)
	{
		perror("fork\n");
		all_free_1(all->test, all->env, all->head_env, all->args);
		exit(EXIT_FAILURE);
	}
}

int is_executable(const char *path) {
    struct stat st;

    if (stat(path, &st) == 0) {
        if (st.st_mode & S_IXUSR || st.st_mode & S_IXGRP || st.st_mode & S_IXOTH) {
            return 1; // Le fichier est exécutable
        } else {
            return 0; // Le fichier n'est pas exécutable
        }
    } else {
        perror("stat");
        return -1; // Erreur
    }
}

void	exec_command_alone(t_all *all)
{
	if (all->pid == 0)
	{
		dup_in_child(all);
		if (applic_bulltin(all->test, all->env, all->head_env, all->args))
		{
			all_free_1(all->test, all->env, all->head_env, all->args);
			exit(0);
		}
		if (!all->args || !all->args[0])
			;
		else if (access(all->args[0], F_OK) || is_executable(all->args[0]) == 0)
		{
			char	*tmp;

			if(all->args[0])
			{
				tmp = ft_strjoin(all->args[0], " : command not found\n");
				write(2, tmp, ft_strlen(tmp));
				free(tmp;)
			}
		}
		else
			execve(all->args[0], all->args, all->envs);
		free_struct_all(all);
		exit(127);
	}
	else
	{
		close(all->pipe_fds[1]);
		if (all->test->next)
			all->prev_pipe_fd = all->pipe_fds[0];
		else
			close(all->pipe_fds[0]);
		free(all->test->cmd);
		free_struct_dir(all->test->dir);
		all->test = all->test->next;
	}
}

void	prepare_n_exit(t_all *all)
{
	all->args = creat_args(all->test, &all->i_a, &all->j_a);
	apply_redirections_to_command_line(all);
	all->str = verif_cmd(all->args, all->envs);
	if (all->str != NULL)
		all->args[0] = ft_strdup(all->str);
	if (!ft_strcmp(all->test->cmd, "exit" ))
	{
		if (ft_exit_1(all->test))
		{
			if (!all->test->arg)
			{
				eof(all->input, all->envs, all->env, all->head_env);
				all_free_1(all->test, all->env, all->head_env, all->args);
				exit(0);
			}
			eof(all->input, all->envs, all->env, all->head_env);
			all_free_1(all->test, all->env, all->head_env, all->args);
			exit(ft_atoi(all->test->arg[0]));
		}
	}
}

void	exec_multi_cmd(t_all *all)
{
	if (!applic_bulltin(all->test, all->env, all->head_env, all->args))
	{
		all->pid = fork();
		if ((all->pid) < 0)
		{
			perror("fork\n");
			all_free_1(all->test, all->env, all->head_env, all->args);
			exit(EXIT_FAILURE);
		}
		else if (all->pid == 0 && !all->args[0])
		{
			free_struct_all(all);
			exit(0);
		}
		else if (all->pid == 0 && !is_bultin(all->args[0]))
		{
			if (!all->args || !all->args[0])
				;
			else if (access(all->args[0], F_OK) || is_executable(all->args[0]) == 0)
				printf("%s: command not found\n", all->test->cmd);
			else
				execve(all->args[0], all->args, all->envs);
			free_struct_all(all);
			exit(127);
		}
	}
}

void	exec_all(t_all *all)
{
	if (all->command_alone > 0)
	{
		all->prev_pipe_fd = -1;
		while (all->test)
		{
			prepare_block(all);
			exec_command_alone(all);
		}
	}
	else
	{
		printf(BACK_RED"MULTI"RST"\n");
		prepare_n_exit(all);
		exec_multi_cmd(all);
		free_string_array(all->envs);
	}
	end_prompt(all);
}
