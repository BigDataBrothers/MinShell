/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 18:58:47 by myassine          #+#    #+#             */
/*   Updated: 2024/02/23 17:14:43 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_struct_dir(t_dir *dir)
{
	t_dir	*tmp;

	if (!dir || dir == NULL)
		return ;
	while (dir)
	{
		tmp = dir;
		dir = dir->next;
		if (tmp->file)
		{
			free(tmp->file);
			tmp->file = NULL;
		}
		free(tmp);
		tmp = NULL;
	}
	free(dir);
}

void	free_block(t_block *block)
{
	if (block)
	{
		if (block->arg)
		{
			free_string_array(block->arg);
			block->arg = NULL;
		}
		if (block->dir)
		{
			free_struct_dir(block->dir);
			block->dir = NULL;
		}
		if (block->cmd != NULL)
		{
			free(block->cmd);
			block->cmd = NULL;
		}
		free(block);
	}
}

void	free_block2(t_block *test)
{
	if (!test || test == NULL)
		return ;
	if (test->arg)
		free_string_array(test->arg);
	if (test->cmd)
		free(test->cmd);
	free(test);
}

void	free_block_list(t_block *head)
{
	t_block	*temp;

	if (!head)
		return ;
	while (head)
	{
		temp = head->next;
		free_block(head);
		head = temp;
	}
	if (head)
		free_block(head);
}

void	free_string_array(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array && array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}

void	terminat(char *input, char **envs, t_env *env, t_env *head_env)
{
	if (input)
		free(input);
	if (env)
		free_env(env, head_env);
	if (envs)
		free_string_array(envs);
}

void	freeme(t_all *all, int action)
{
	if (all->input)
		free(all->input);
	all->input = NULL;
	if (all->str)
		free(all->str);
	all->str = NULL;
	if (all->args)
		free_string_array(all->args);
	all->args = NULL;
	if (action)
	{
		if (all->envs)
			free_string_array(all->envs);
		if (all->env)
			free_env(all->env, all->head_env);
	}
	if (all->test)
	{
		free_block_list(all->test);
		all->head = NULL;
		all->test = NULL;
	}
}
