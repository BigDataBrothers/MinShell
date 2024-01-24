/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 21:16:58 by myassine          #+#    #+#             */
/*   Updated: 2024/01/24 19:31:23 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_block	*new_block(void)
{
	t_block	*new;

	new = (t_block *)malloc(sizeof(t_block));
	if (!new)
		return (NULL);
	new->cmd = NULL;
	new->arg = NULL;
	new->next = NULL;
	new->dir = NULL;
	new->dir_head = NULL;
	return (new);
}

char	**add_new_block_tab(char ***tab, char *str)
{
	int		i;
	char	**tmp;

	i = 0;
	tmp = NULL;
	while (tab && (*tab) && (*tab)[i])
		i++;
	tmp = (char **)malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (tab && *tab && (*tab)[i])
	{
		tmp[i] = (*tab)[i];
		i++;
	}
	tmp[i++] = str;
	tmp[i] = NULL;
	*tab = tmp;
	return (*tab);
}

t_dir	*malloc_dir(void)
{
	t_dir	*dir;

	dir = (t_dir *)malloc(sizeof(t_dir));
	if (!dir)
		return (NULL);
	memset(dir, 0, sizeof(t_dir));
	return (dir);
}

t_dir	*new_dir(char *redir, char *target, t_dir *dir)
{
	if (!dir)
		dir = malloc_dir();
	(void)redir;
	(void)target;
	return (dir);
}

void	free_env(t_env *env, t_env *head_env)
{
	while (env)
	{
		head_env = env->next;
		if (env->str && env->str[0])
		{
			free(env->str[0]);
			env->str[0] = NULL;
		}
		if (env->str && env->str[1])
		{
			free(env->str[1]);
			env->str[1] = NULL;
		}
		if (env)
			free(env);
		env = NULL;
		env = head_env;
	}
	env = NULL;
}
