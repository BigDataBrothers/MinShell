/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 18:43:20 by myassine          #+#    #+#             */
/*   Updated: 2024/02/23 17:10:25 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	allocate_and_assign_arguments(char **token, int *j, t_block *tmp)
{
	int		v;
	int		argc;
	char	**args;
	int		i;

	i = *j;
	argc = nbr_arg(token, (i));
	if (argc == -1)
		return ;
	args = (char **)malloc(sizeof(char *) * (argc + 2));
	if (!args)
		return ;
	v = 0;
	while (token[i])
	{
		if (is_redir(token[i]))
		{
			process_redirection_token(token, j, tmp);
			i++;
			(*j)++;
		}
		else if (!tmp->cmd)
			process_command_token(token, j, tmp);
		else
			args[v++] = ft_strdup(token[i]);
		i++;
	}
	*j = i;
	args[v] = '\0';
	tmp->arg = args;
}

int	treat_token(char **token, int *j, t_block *tmp)
{
	if (!tmp->cmd && !is_redir(token[*j]))
		process_command_token(token, j, tmp);
	else
		allocate_and_assign_arguments(token, j, tmp);
	return (0);
}

void	free_start_dir(t_dir *dir)
{
	if (!dir || dir == NULL)
		return ;
	while (dir)
	{
		if (dir->next->file == NULL)
		{
			free(dir->next);
			dir->next = NULL;
			break ;
		}
		dir = dir->next;
	}
}

int	treat_cmd_line(char *cmd_line, t_block *tmp)
{
	char	**token;
	int		j;
	t_dir	*head;

	j = 0;
	token = ft_split_path(cmd_line, ' ');
	tmp->dir = create_new_dir();
	if (!tmp->dir)
		return (FAILURE);
	head = tmp->dir;
	while (token[j] != NULL)
		if (treat_token(token, &j, tmp))
			return (1);
	tmp->dir = head;
	if (tmp->dir && tmp->dir->file == NULL)
		return (free_string_array(token), 0);
	free_start_dir(tmp->dir);
	tmp->dir = head;
	free_string_array(token);
	return (0);
}

void	add_to_special(char *result, char *str, int *i, int *j)
{
	if (str[*i] == '>' || str[*i] == '<' || str[*i] == '|')
	{
		result[(*j)++] = ' ';
		result[(*j)++] = str[(*i)++];
		if (str[*i] == '>' || str[*i] == '<')
			result[(*j)++] = str[(*i)++];
		result[(*j)++] = ' ';
	}
	else if (str[*i] == ' ')
	{
		result[(*j)++] = str[(*i)++];
		while (str[*i] == ' ')
			(*i)++;
	}
	else
		result[(*j)++] = str[(*i)++];
}
