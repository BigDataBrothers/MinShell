/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 17:41:49 by myassine          #+#    #+#             */
/*   Updated: 2024/01/16 19:42:29 by myassine         ###   ########.fr       */
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
		else if (access(all->args[0], F_OK))
			dprintf(2, "%s: command not found\n", all->test->cmd);//Ecrir sur sortie d erreur
		else
			execve(all->args[0], all->args, all->envs);
		all_free_1(all->test, all->env, all->head_env, all->args);
		exit(127);
	}
	else
	{
		close(all->pipe_fds[1]);
		if (all->test->next)
			all->prev_pipe_fd = all->pipe_fds[0];
		else
			close(all->pipe_fds[0]);
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
			else if (access(all->args[0], F_OK))
				write(2, "123\n", 4);
				//printf("%s: command not found\n", all->test->cmd);
			else
				execve(all->args[0], all->args, all->envs);
			// if (all->args[0] && all->test->cmd)
			// 	printf("%s: command not found\n", all->args[0]);
			// all_free_1(all->test, all->env, all->head_env, all->args);
			free_struct_all(all);
			exit(127);
		}
	}
}

void	exec_all(t_all *all)
{
	// t_block *tmp = all->test;
	
	// printf(GREEN"all->command: %d"RESET"\n", all->command_alone);
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
	// all->test = tmp;
	end_prompt(all);
}
