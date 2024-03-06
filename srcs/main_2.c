/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 16:34:22 by myassine          #+#    #+#             */
/*   Updated: 2024/02/29 17:19:54 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*remove_char_at_index(char *str, int i)
{
	char	*new_str;
	int		j;
	int		k;

	j = 0;
	k = 0;
	new_str = (char *)ft_calloc(ft_strlen(str), 1);
	if (str == NULL || i < 0)
		return (NULL);
	if (new_str == NULL)
		return (NULL);
	while (str[k] != '\0')
	{
		if (k != i)
			new_str[j++] = str[k];
		k++;
	}
	return (new_str);
}

void	prepare_block(t_all *all)
{
	if (all->args)
		free_string_array(all->args);
	all->args = creat_args(all->test, &all->i_a, &all->j_a);
	all->str = verif_cmd(all->args, all->envs);
	if (all->str != NULL)
	{
		free(all->args[0]);
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

bool	alloc_and_assign_args_2(char **token, int *i, int *j, t_block *tmp)
{
	if (is_redir(token[*i]))
	{
		process_redirection_token(token, i, j, tmp);
		(*i)++;
		(*j)++;
		return (true);
	}
	return (false);
}
