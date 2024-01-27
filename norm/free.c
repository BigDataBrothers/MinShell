/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 18:58:47 by myassine          #+#    #+#             */
/*   Updated: 2024/01/26 20:29:24 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_struct_dir(t_dir *dir)
{
	t_dir	*tmp;

	if (!dir)
		return ;
	while (dir)
	{
		tmp = dir;
		dir = dir->next;
		free(tmp->file);
		tmp->file = NULL;
		free(tmp);
		tmp = NULL;
	}
}

void	free_block(t_block *block)
{
	if (block)
	{
		if (block->cmd)
		{
			free(block->cmd);
			block->cmd = NULL;
		}
		// free_string_array(block->arg);
		block->arg = NULL;
		free_struct_dir(block->dir);
		
		block->dir = NULL;
		free(block);
		block = NULL;
	}
}

void	free_block_list(t_block *head)
{
	t_block	*temp;

	if (!head)
		return ;
	while (head)
	{
		temp = head;
		head = head->next;
		free_block(temp);
		temp = NULL;
	}
}

void	free_string_array(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array && array[i])
	{
		// dprintf(2, "pointer = %p\n", array[i]);
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
	(void)envs;
	// if (envs)
	// 	free_string_array(envs);
}
