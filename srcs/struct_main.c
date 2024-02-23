/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 17:41:49 by myassine          #+#    #+#             */
/*   Updated: 2024/02/23 18:59:24 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	prepare_block(t_all *all)
{
	if (all->args)
		free_string_array(all->args);
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
		perror("pipe\n");
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

void	free_one_block(t_block *block)
{
	int		i;
	t_dir	*tmp;

	(void)i;
	(void)block;
	i = -1;
	while (block->arg && block->arg[++i])
		free(block->arg[i]);
	free(block->cmd);
	tmp = block->dir;
	while (block->dir != NULL)
	{
		free(block->dir->file);
		tmp = block->dir;
		block->dir = block->dir->next;
		free(tmp);
	}
}

int	len_z_tab(char *str)
{
	int	len;
	int	it;

	len = 0;
	it = -1;
	while (str[++it])
		if (str[it] != '\t')
			len++;
	return (len);
}

char	*mouv_tab(char *cmd)
{
	char	*n_cmd;
	int		it;
	int		it2;

	it = 0;
	it2 = 0;
	n_cmd = malloc(sizeof(char) * len_z_tab(cmd) + 1);
	if (!n_cmd)
		return (NULL);
	while (cmd[it])
	{
		if (cmd[it] != '\t')
			n_cmd[it2++] = cmd[it];
		it++;
	}
	n_cmd[it2] = '\0';
	return (n_cmd);
}

void	exec_multi_cmd(t_all *all, int i)
{
	char	*n_cmd;
;
	n_cmd = NULL;
	signal(SIGINT, &rsigint_handle);
	if (all->pid == 0)
	{
		signal(SIGINT, &rsigint_handle);
		signal(SIGQUIT, &sig_antislash);
		dup_in_child(all);
		if (aplic_bulltin(all, all->env, all->head_env, all->args))
		{
			freeme(all, 1);
			exit(0);
		}
		if (all->args[0] && !g_ctrl_c)
		{
			execve(all->args[0], all->args, all->envs);
		}
		if (n_cmd)
			free(n_cmd);
		if (!all->erreur && all->test->cmd && !g_ctrl_c)
		{
			n_cmd = mouv_tab(all->test->cmd);
			write(2, n_cmd, ft_strlen(n_cmd));
			write(2, ": command not found\n", 20);
			free(n_cmd);
		}
		close_saved(all);
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
		all->args[0] = ft_strdupf(all->str);
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

void	exec_all(t_all *all)
{
	t_block	*tmp;
	t_block	*tmp1;
	int		i;

	i = 0;
	if (all->command_alone < 1 && is_bultin(all->test->cmd))
	{
		prepare_n_exit(all);
		aplic_bulltin(all, all->env, all->head_env, all->args);
		return ;
	}
	else
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
