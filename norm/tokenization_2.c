/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 18:43:20 by myassine          #+#    #+#             */
/*   Updated: 2024/01/28 21:37:40 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	allocate_and_assign_arguments(char **token, int *j, t_block *tmp)
{
	int		v;
	int		argc;
	char	**args = NULL;
	int l = *j;
//	(void)l;
	// printf("----------------- j : [%d]--------------\n", *j);
	argc = nbr_arg(token, (*j));
	if (argc == -1)
		return ;
	args = ft_calloc(sizeof(char **),  (argc + 1));
	if (!args)
		return ;
	v = 0;
	// printf("------------------------token in allocate : [%s]----------------\n", token[0]);
	while (token[*j])
	{
		if (token[*j] == NULL)
			break ;
		// printf("------------------------token in allocate with j: [%s]----------------\n", token[*j]);
		if (is_redir(token[*j]) && token[*j] != NULL)
			process_redirection_token(token, j, tmp);
		else if (!tmp->cmd)
			process_command_token(token, j, tmp);
		else
		{
			args[v++] = ft_strdup(token[(*j)++]);
		}
		// printf("------------------------token in allocate with j: [%s]----------------\n", token[*j]);
		// l = *j;
		l++;
	}
	args[v] = '\0';
	tmp->arg = args;
}

int	treat_token(char **token, int *j, t_block *tmp)
{
	if (token && token[*j] && tmp && !tmp->cmd && !is_redir(token[*j]))
		process_command_token(token, j, tmp);
	else
		allocate_and_assign_arguments(token, j, tmp);
	return (0);
}

void	free_start_dir(t_dir *dir)
{
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
	// int		*l;
	t_dir	*head;

	j = 0;
	printf(BACK_BLUE"cmd_line: %s"RST"\n", cmd_line);
	token = ft_split_path(cmd_line, ' ');
	if(!token)
		return (1);
	while(token[j])
		printf(BACK_PURPLE"token[j]: %s"RST"\n", token[j++]);
	j = 0;
	tmp->dir = create_new_dir();
	if (!tmp->dir)
		return (FAILURE);
	head = tmp->dir;
	printf(BACK_RED"token[j]: %s"RST"\n", token[j]);
	while (token && token[j] != NULL && tmp)
	{
		printf(BOLD_CYAN"--------------------token [%s]-------------\n", token[j]);
		printf(BACK_CYAN"tmp->cmd: %s"RST"\n", tmp->cmd);
		if (treat_token(token, &j, tmp))
			return (free_string_array(token), 1);
	}
	j = 0;
	while(token[j++])
		printf(BACK_YELLOW"token[j]: %s\n&token[j]: %p"RST"\n", token[j], token[j]);
	free_string_array(token);
	tmp->dir = head;
	if (tmp->dir && tmp->dir->file == NULL)
		return (0);
	tmp->dir = head;
	tmp->dir_head = head;
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
