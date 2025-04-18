/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 17:41:49 by myassine          #+#    #+#             */
/*   Updated: 2024/03/03 20:45:40 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	exec(t_all *all, char *n_cmd)
{
	if (aplic_bulltin(all, all->env, all->head_env, all->args))
	{
		freeme(all, 1);
		close_saved(all);
		exit(0);
	}
	if (all->args[0] && g_ctrl_c != 130)
		execve(all->args[0], all->args, all->envs);
	if (n_cmd)
		free(n_cmd);
	if (!all->erreur && all->test->cmd && !g_ctrl_c)
	{
		n_cmd = mouv_tab(all->test->cmd);
		write(2, n_cmd, ft_strlen(n_cmd));
		write(2, ": command not found\n", 20);
		free(n_cmd);
	}
}

void	exec_multi_cmd(t_all *all, int i)
{
	char	*n_cmd;

	n_cmd = NULL;
	signal(SIGINT, &rsigint_handle);
	if (all->pid == 0)
	{
		signal(SIGINT, &rsigint_handle);
		signal(SIGQUIT, &sig_antislash);
		dup_in_child(all);
		exec(all, n_cmd);
		freeme(all, 1);
		exit(127);
	}
	else
	{
		close(all->pipe_fds[1]);
		if (i)
			close(all->prev_pipe_fd);
		all->prev_pipe_fd = all->pipe_fds[0];
		signal(SIGQUIT, SIG_IGN);
	}
	all->test = all->test->next;
}

void	prepare_n_exit(t_all *all)
{
	int	atoi;

	if (all->args)
		free_string_array(all->args);
	all->args = creat_args(all->test, &all->i_a, &all->j_a);
	all->str = verif_cmd(all->args, all->envs);
	apply_redirections_to_command_line(all);
	if (all->str != NULL)
	{
		free(all->args[0]);
		all->args[0] = ft_strdupf(all->str);
		all->str = NULL;
	}
	if (!ft_strcmp(all->test->cmd, "exit"))
	{
		if (ft_exit_1(all->test))
		{
			atoi = 0;
			if (all->test->arg)
				atoi = ft_atoi(all->test->arg[0]);
			close_saved(all);
			freeme(all, 1);
			exit(atoi);
		}
	}
}

int	exec_all_2(t_all *all)
{
	if (all->command_alone < 1 && is_bultin(all->test->cmd))
	{
		all->pid = -1;
		prepare_n_exit(all);
		if (all->i_a != -1 && all->j_a != -1)
			aplic_bulltin(all, all->env, all->head_env, all->args);
		return (0);
	}
	return (1);
}

void	exec_all(t_all *all)
{
	t_block	*tmp;
	t_block	*tmp1;
	int		i;

	i = 0;
	if (exec_all_2(all))
	{
		all->prev_pipe_fd = -1;
		all->head = all->test;
		tmp = all->test;
		while (tmp)
		{
			prepare_block(all);
			exec_multi_cmd(all, i);
			tmp1 = tmp;
			tmp = tmp->next;
			free_block(tmp1);
			tmp1 = NULL;
			i++;
		}
		free_block(tmp1);
		close(all->pipe_fds[0]);
	}
	end_prompt(all);
}
