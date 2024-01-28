/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 17:41:49 by myassine          #+#    #+#             */
/*   Updated: 2024/01/28 23:06:04 by myassine         ###   ########.fr       */
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
		all->str = NULL;
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
	// if (all->args && all->args[0])
	// 	free(all->args[0]);
	// all->args[0] = NULL;
}

int	is_executable(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
	{
		if (st.st_mode & S_IXUSR || st.st_mode & S_IXGRP || st.st_mode & S_IXOTH)
			return (1);
		else
			return (0);
	}
	else
	{
		perror("stat");
		return (-1);
	}
}

void	exec_command_alone(t_all *all)
{
	char	*tmp;

	if (all->pid == 0)
	{
		dup_in_child(all);
		if (applic_bulltin(all))
		{
			all_free_1(all->test, all->env, all->head_env, all->args);
			exit(0);
		}
		if (!all->args || !all->args[0])
			;
		else if (access(all->args[0], F_OK) || is_executable(all->args[0]) == 0)
		{
			if (all->args[0])
			{
				tmp = ft_strjoin(all->args[0], " : command not found\n");
				write(2, tmp, ft_strlen(tmp));
				free(tmp);
			}
		}
		else
			execve(all->args[0], all->args, all->envs);
		free_env(all->env, all->head_env);
		free_struct_all(all);
		all->ret_val = 127;
		exit(127);
	}
	else
	{
		close(all->pipe_fds[1]);
		if (all->test->next)
			all->prev_pipe_fd = all->pipe_fds[0];
		else
			close(all->pipe_fds[0]);
		//free_string_array(all->args);
		free(all->test->cmd);
		all->test->cmd = NULL;
		free_struct_dir(all->test->dir);
		all->test->dir = NULL;
		all->test = all->test->next;
	}
}

void free_string_arrayss(char ***array)
{
    int i;

    i = 0;
    if (*array == NULL)
        return;
    while (*array && (*array)[i])
    {
        free((*array)[i]);
        (*array)[i] = NULL;
        i++;
    }
    free(*array);
    *array = NULL;
}


void	prepare_n_exit(t_all *all)
{
	if (all->cnt != 0)
	{
		free_string_array(all->args);
		all->args = NULL;
	}
	all->args = creat_args(all->test, &all->i_a, &all->j_a);
	apply_redirections_to_command_line(all);
	all->str = verif_cmd(all->args, all->envs);
	if (all->str != NULL)
	{
		if(all->args[0])
			free(all->args[0]);
		all->args[0] = ft_strdupf(all->str);
	}
	if (!ft_strcmp(all->test->cmd, "exit" ))
	{
		if (ft_exit_1(all->test))
		{
			if (!all->test->arg)
			{
				if (all->envs)
					free_string_array(all->envs);
				printf("exit\n");
				eof(all->input, all->envs, all->env, all->head_env);
				free(all->tmp_path);
				all->tmp_path = NULL;
				free_string_array(all->args);
				free_string_array(all->test->arg);
				// free_block_list(all->test_head->arg);
				all_free_1(all->test, all->env, all->head_env, all->args);
				exit(0);
			}
			int exit_fd;
		
			exit_fd  = ft_atoi(all->test->arg[0]);
			if (all->envs)
				free_string_array(all->envs);
			eof(all->input, all->envs, all->env, all->head_env);
			free(all->tmp_path);
			all->tmp_path = NULL;
			free_string_array(all->args);
			all->args = NULL;
			all_free_1(all->test, all->env, all->head_env, all->args);
			exit(exit_fd);
		}
	}
}



char	*ft_check_abs_cmd(t_all *all)
{
	if (all->args[0][0] == '/')
		return (all->args[0]);
	else
		return (all->test->cmd);
}

void	exec_multi_cmd(t_all *all)
{
	all->ret_val = 0;
	
	if (!applic_bulltin(all))
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
			else if (access(ft_check_abs_cmd(all), F_OK) || is_executable(ft_check_abs_cmd(all)) == 0)
			{
				export_env_1(all, "?=127", 0);
				all->ret_val = 127;
				printf("%s: command nots found\n", all->args[0]);
				free_env(all->env, all->head_env);
			}
			else
				execve(ft_check_abs_cmd(all), all->args, all->envs);
			free_struct_all(all);
			exit(127);
		}
	}
}

void	exec_all(t_all *all)
{
	all->status = -1;
	if (all->command_alone > 0)
	{
		all->prev_pipe_fd = -1;
		while (all->test)
		{
			prepare_block(all);
			exec_command_alone(all);
			// exec_multi_cmd(all);
		}
	}
	else
	{
		prepare_n_exit(all);
		exec_multi_cmd(all);
	}
	end_prompt(all);
}
