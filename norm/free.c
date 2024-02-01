/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 18:58:47 by myassine          #+#    #+#             */
/*   Updated: 2024/02/01 22:48:22 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_struct_dir(t_dir *dir)
{
	t_dir *tmp;

	// printf("dir == %p", dir);
	while (dir)
	{
		tmp = dir;
		dir = dir->next;
		// printf("file == %s", tmp->file);
		free(tmp->file);
		free(tmp);
	}
}

void	free_block(t_block *block)
{
	if (block)
	{
		if (block->cmd)
		{
			printf(BACK_RED"TEST"RST"\n");
			free(block->cmd);
			printf(CYAN"block->cmd: %p"RESET"\n", block->cmd);
			block->cmd = NULL;
		}
		if(block->arg)
			free_string_array(block->arg);
		printf(CYAN"block->arg: %p"RESET"\n", block->arg);
		// printf(BACK_GREEN"block->arg[0]: %s"RST"\n", block->arg[0]);
		block->arg = NULL;
		free_struct_dir(block->dir);
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
