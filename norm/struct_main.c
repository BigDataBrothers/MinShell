/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 17:41:49 by myassine          #+#    #+#             */
/*   Updated: 2024/02/08 01:14:31 by myassine         ###   ########.fr       */
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


void free_one_block(t_block *block)
{
	int i;

	(void)i;
	(void)block;
	i = -1;
	while(block->arg && block->arg[++i])
		free(block->arg[i]);
	free(block->cmd);
	t_dir *tmp = block->dir;
   	// t_dir *temp;

   while (block->dir != NULL)
    {
		free(block->dir->file);
       tmp = block->dir;
       block->dir = block->dir->next;
       free(tmp);
    }
	// while (tmp)
	// {
	// 	free(tmp->file);
	// 	tmp->file = NULL;
	// 	tmp = tmp->next;
	// }
	// free_start_dir(block->dir);
}

void	exec_multi_cmd(t_all *all, int i)
{
	if (all->pid == 0)
	{
		signal(SIGINT, &sigint_handler);
		signal(SIGQUIT, &sig_antislash);
		dup_in_child(all);
		if (applic_bulltin(all, all->env, all->head_env, all->args))
		{
			all_free_1(all->test, all->env, all->head_env, all->args);
			exit(0);
		}
		execve(all->args[0], all->args, all->envs);
		if(!all->erreur)
		{
			write(2, all->test->cmd, ft_strlen(all->test->cmd));
			write(2, ": command not found\n", 20);
		}
		close_saved(all);
		all_free_1(all->test, all->env, all->head_env, all->args);
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
	t_block *tmp;
	tmp = all->test;
	all->test = all->test->next;
	free_one_block(tmp);
}

void	prepare_n_exit(t_all *all)
{
	all->args = creat_args(all->test, &all->i_a, &all->j_a);
	all->str = verif_cmd(all->args, all->envs);
	apply_redirections_to_command_line(all);
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
			exit(ft_atoi(all->test->arg[0]));
		}
	}
} 

void	exec_all(t_all *all)
{
	int i = 0;
	if (all->command_alone < 1 && is_bultin(all->test->cmd))
	{
		prepare_n_exit(all);
		applic_bulltin(all, all->env, all->head_env, all->args);
		free_string_array(all->envs);
		free_struct_all(all);

		return ;
	}
	else
	{
		all->prev_pipe_fd = -1;
		t_block *tmp = all->test;
		while (tmp)
		{

			prepare_block(all);
			exec_multi_cmd(all, i);
			tmp = tmp->next;
			i++;
		}
		close(all->pipe_fds[0]);
	}
	end_prompt(all);
}

// UNE COMMANDE && BUILTIN
// 	exec sans fork
// SINON echo | ls
// 	exec avec fork