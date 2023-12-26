/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 18:58:47 by myassine          #+#    #+#             */
/*   Updated: 2023/12/26 21:01:55 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void free_dir(t_dir *dir)
{
    while(dir)
    {
        if(dir->file)
        {
            free(dir->file);
            dir->file = NULL;
        }
        dir = dir->next;
    }
}

void free_block(t_block *block)
{
    if (block)
    {
        if (block->cmd)
            free(block->cmd);
        if (block->arg)
        {
            char **arg = block->arg;
            while (*arg)
            {
                free(*arg);
                // arg = NULL;
                arg++;
            }
            free(block->arg);
            block->arg = NULL;
        }
        // printf("block->dir: %p\n", block->dir);
        free(block->dir);
        // free_dir(block->dir);
        free(block);
        block = NULL;
    }
}

// Fonction pour libérer la mémoire associée à une liste de structures t_block
void free_block_list(t_block *head)
{
    if (!head)
        return;
    while (head)
    {
        t_block *temp = head;
        head = head->next;
        free_block(temp);
    }
}

void freeStringArray(char **array) 
{
	int	i;

	i = 0;
    if (array == NULL)
        return;
    while(array && array[i])
	{
        free(array[i]);
		array[i] = NULL;
		i++;
    }
    free(array);
	array = NULL;
}

void freeStringArrays(char ***arrays)
{
    if (arrays == NULL)
        return;
    int i = 0;
    char ***array = &arrays[i];
    while (*array != NULL)
	{
        char **currentElement = *array;
        while (*currentElement != NULL)
		{
            char *currentString = *currentElement;
            free(currentString);
            currentElement++;
        }
        free(*array);
        i++;
        array = &arrays[i];
    }
    free(arrays);
}

void	terminat(char *input, char **envs, t_env *env, t_env *head_env)
{
	if(input)
		free(input);
	if(env)
		free_env(env, head_env);
	if(envs)
		freeStringArray(envs);
}
