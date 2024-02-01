/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 17:41:49 by myassine          #+#    #+#             */
/*   Updated: 2024/02/01 22:19:13 by myassine         ###   ########.fr       */
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

void	exec_multi_cmd(t_all *all)
{
	if (all->pid == 0)
	{
		dup_in_child(all);
		if (applic_bulltin(all->test, all->env, all->head_env, all->args))
		{
			all_free_1(all->test, all->env, all->head_env, all->args);
			exit(0);
		}
		else if (execve(all->args[0], all->args, all->envs) == -1)
		{
			if (all->test->cmd)
				printf("%s: command not found\n", all->test->cmd);
			all_free_1(all->test, all->env, all->head_env, all->args);
			exit(127);
		}
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
		all->args[0] = ft_strdupf(all->str);
	if (!ft_strcmp(all->test->cmd, "exit"))
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
			// printf(PURPLE"all->test: %p\n", all->test);
			// printf(PURPLE"all->test->arg: %p\n", all->test->arg);
			// printf(PURPLE"all->test->arg[0]: %p\n", all->test->arg[0]);
			// check all->test->arg is at NULL
			exit(ft_atoi(all->test->arg[0]));
		}
	}
}

void	exec_alone_command(t_all *all)
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
		else if (all->pid == 0 && !is_bultin(all->args[0]) && \
		execve(all->args[0], all->args, all->envs) == -1)
		{
			if (all->args[0] && all->test->cmd)
				printf("%s: command not found\n", all->args[0]);
			all_free_1(all->test, all->env, all->head_env, all->args);
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
			exec_multi_cmd(all);
		}
	}
	else
	{
		prepare_n_exit(all);
		exec_alone_command(all);
		free_string_array(all->envs);
	}
	end_prompt(all);
}
