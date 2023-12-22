/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 21:39:12 by myassine          #+#    #+#             */
/*   Updated: 2023/12/22 17:46:48 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_dir *create_new_dir()
{
	t_dir *new_dir;

	new_dir = malloc(sizeof(t_dir));
	if (!new_dir)
		return (FAILURE);
	new_dir->file = NULL;
	new_dir->fd = -1;
	new_dir->type = -1;
	return (new_dir);
}

void choose_type_redir(char **token, int j, t_dir *new_dir)
{
	if (!ft_strcmp(token[j], ">>"))
		new_dir->type = APPEND;
	else if (!ft_strcmp(token[j], ">"))
		new_dir->type = OUT;
	else if (!ft_strcmp(token[j], "<<"))
		new_dir->type = HEREDOC;
	else if (!ft_strcmp(token[j], "<"))
		new_dir->type = IN;
}

// [switcher type de retour 1 -> FAILURE]
int treat_token(char **token, int *j, t_block *tmp, int *count)
{
	int argc;
	int v = 0;

	v = 0;
	if ((*count) == 0 && !is_redir(token[*j]))
	{
		tmp->cmd = ft_strdup(token[*j]);
		(*count)++;
		(*j)++;
	}
	else
	{
		argc = nbr_arg(token, (*j), count);
		if (argc == -1)
			return (1);
		char **args = (char **)malloc(sizeof(char *) * (argc + 1));
		if (!args)
			return (1);
		while (token[*j])
		{
			if(is_redir(token[*j]))
			{
				choose_type_redir(token, *j, tmp->dir);
				(*j)++;
				tmp->dir->file = ft_strdup(token[*j]);
				tmp->dir->fd = -1;
				tmp->dir->next = create_new_dir();
				if (!tmp->dir->next)
					return (1);
				tmp->dir = tmp->dir->next;
			}
			else if(*count == 0)
			{
				tmp->cmd = ft_strdup(token[*j]);//
				(*count)++;
			}
			else
			{
				args[v] = ft_strdup(token[*j]);
				v++;
			}
			(*j)++;
		}
		args[v] = NULL;
		tmp->arg = args;
	}
	return (0);
}

int	treat_cmd_line(char *cmd_line, t_block *tmp)
{
	char	**token;
	int		j;
	int		count;
	t_dir	*head;
	count = 0;
	
	j = 0;
	token = ft_split_path(cmd_line, ' ');
	tmp->dir = create_new_dir();
	if (!tmp->dir)
		return (FAILURE);
	head = tmp->dir;
	while (token[j] != NULL)
		if (treat_token(token, &j, tmp, &count))
			return (1);

	// [START CLEAN LAST POINTER LISTE CHAINEE T_DIR*]
	tmp->dir = head;
	if (tmp->dir && tmp->dir->file == NULL)
		return (freeStringArray(token), 0);
	while (tmp->dir)
	{
		if (tmp->dir->next->file == NULL)
		{
			free(tmp->dir->next);
			tmp->dir->next = NULL;
			break ;
		}
		tmp->dir = tmp->dir->next;
	}
	tmp->dir = head;
	// [END CLEAN LAST POINTER LISTE CHAINEE T_DIR*]
	freeStringArray(token);
	return (0);
}

char	*add_spaces(char *str)
{
	char	*result;
	int		i;
	int		j;

	result = (char *)malloc(sizeof(char) * (strlen(str) * 2 + 1));
	if (result == NULL)
		return (NULL);
	i = 0;
	j = i;
	while (str[i] != '\0')
	{
		if (str[i] == '>' || str[i] == '<' || str[i] == '|')
		{
			result[j++] = ' ';
			result[j++] = str[i++];
			if (str[i] == '>' || str[i] == '<')
				result[j++] = str[i++];
			result[j++] = ' ';
		}
		else if (str[i] == ' ')
		{
			result[j++] = str[i++];
			while (str[i] == ' ')
				i++;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

t_block *tokenization(char *input)
{
	char	**split_input;
	t_block *original;
	t_block *tmp;
	int		i;

	i = 0;
	original = new_block();
	tmp = original;
	char *tmp_input = ft_strdup(input);
	free(input);
	input = NULL;
	input = add_spaces(tmp_input);
	free(tmp_input);
	split_input = ft_split_path(input, '|');
	while (split_input[i])
	{
		tmp->cmd = NULL;
		tmp->arg = NULL;
		tmp->next = NULL;
		tmp->dir = NULL;
		tmp->pipe_out = STDOUT_FILENO;
		tmp->pipe_in = STDIN_FILENO;
		if (treat_cmd_line(split_input[i], tmp)) // PIPELINE
			return (NULL);
		if (split_input[i + 1])
		    tmp->next = new_block();
		if (tmp->next)
		    tmp = tmp->next;
		i++;
    }
    tmp->next = NULL;
	if(split_input)
		freeStringArray(split_input);
    return (original);
}