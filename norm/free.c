/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 18:58:47 by myassine          #+#    #+#             */
/*   Updated: 2023/12/27 19:47:03 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_block(t_block *block)
{
	char	**arg;

	if (block)
	{
		if (block->cmd)
			free(block->cmd);
		if (block->arg)
		{
			arg = block->arg;
			while (*arg)
			{
				free(*arg);
				arg++;
			}
			free(block->arg);
			block->arg = NULL;
		}
		free(block->dir);
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
