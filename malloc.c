/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 21:16:58 by myassine          #+#    #+#             */
/*   Updated: 2023/12/09 19:29:03 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_block *new_block(void)
{
	t_block *new = (t_block *)malloc(sizeof(t_block));
	if (!new) {
		return NULL;
	}
	
	new->cmd = NULL;
	new->arg = NULL;
	new->next = NULL;
	new->dir = NULL; 

	return new;
}


char **add_new_block_tab(char ***tab, char *str)
{
	int i = 0;
	char **tmp;

	tmp = NULL;
	while (tab && (*tab) && (*tab)[i])
		i++;
	tmp = (char **)malloc(sizeof(char*) * (i + 2));
	i = 0;
	while (tab && *tab && (*tab)[i])
	{
		tmp[i] = (*tab)[i];
		i++;
	}
	tmp[i++] = str;
	tmp[i] = NULL;
	//free(tab);
	*tab = tmp;
	return (*tab);
}

t_dir *malloc_dir()
{
	t_dir *dir;

	dir = (t_dir *)malloc(sizeof(t_dir));
	if (!dir)
		return (NULL);
	memset(dir, 0, sizeof(t_dir));
	dir->app_redir_doc = malloc(sizeof(char **) * 3);
	dir->app_redir_doc[FILES] = NULL;
	dir->app_redir_doc[REDIR] = NULL;
	dir->app_redir_doc[NONE] = NULL;
	return(dir);
}

t_dir *new_dir(char *redir, char *target, t_dir *dir)
{
	if (!dir)
		dir = malloc_dir();
	add_new_block_tab(dir->app_redir_doc + FILES, target);
	add_new_block_tab(dir->app_redir_doc + REDIR, redir);
	return dir;
}

// char	**tab_arg(t_block *block, char **token, int j)
// {
// 	block->arg = malloc(sizeof(char *) * (nbr_arg(token, j) + 1));
// 	if (!block->arg)
// 		return (NULL);
// 	return (block->arg);
// }