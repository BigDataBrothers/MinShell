/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 17:41:49 by myassine          #+#    #+#             */
/*   Updated: 2024/01/11 18:43:18 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_all(t_all *all, char **envp)
{
	all->head_env = NULL;
	all->env = NULL;
	all->env = init_env(all->env, all->head_env, envp);
	all->input = NULL;
	all->envs = NULL;
	all->path = NULL;
	all->str = NULL;
	all->tmp_path = NULL;
	all->args = NULL;
	all->pipe_fds[0] = -1; // maybe not init
	all->pipe_fds[1] = -1; // maybe not init
	all->prev_pipe_fd = 0;
	all->saved_stdout = 0;
	all->saved_stdin = 0;
	all->command_alone = 0;
	all->i_a = 0;
	all->j_a = 0;
	all->pid = -1;
	all->status = 0;
}

char	*ft_strdupf(char *s)
{
	int		len;
	int		i;
	char	*d;

	i = 0;
	if (s == NULL)
		return (NULL);
	len = ft_strlen(s);
	d = malloc(sizeof(char) * (len + 1));
	if (!d)
		return (NULL);
	while (s[i] != '\0')
	{
		d[i] = s[i];
		i++;
	}
	d[i] = '\0';
	free(s);
	return (d);
}
