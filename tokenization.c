/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkermia <dkermia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 21:39:12 by myassine          #+#    #+#             */
/*   Updated: 2023/12/28 19:51:17 by dkermia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_dir	*create_new_dir(void)
{
	t_dir	*new_dir;

	new_dir = malloc(sizeof(t_dir));
	if (!new_dir)
		return (FAILURE);
	new_dir->file = NULL;
	new_dir->fd = -1;
	new_dir->type = -1;
	return (new_dir);
}

void	choose_type_redir(char **token, int j, t_dir *new_dir)
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
// Function to handle the command and the first token
void	handle_command_token(char **token, int *j, t_block *tmp, int *count)
{
	if ((*count) == 0 && !is_redir(token[*j]))
	{
		tmp->cmd = ft_strdup(token[*j]);
		(*count)++;
		(*j)++;
	}
}
// Function to handle arguments and redirection
void	handle_args_and_redir(char **token, int *j, t_block *tmp, int *count)
{
	int		argc;
	char	**args;
	int		v;

	v = 0;
	argc = nbr_arg(token, (*j), count);
	if (argc == -1)
		return ;
	args = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!args)
		return ;
	while (token[*j])
	{
		if (is_redir(token[*j]))
		{
			choose_type_redir(token, *j, tmp->dir);
			(*j)++;
			tmp->dir->file = ft_strdup(token[*j]);
			tmp->dir->fd = -1;
			tmp->dir->next = create_new_dir();
			if (!tmp->dir->next)
				return ;
			tmp->dir = tmp->dir->next;
		}
		else if (*count == 0)
		{
			tmp->cmd = ft_strdup(token[*j]);
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

// Main function to coordinate the work of the two functions
int	treat_token(char **token, int *j, t_block *tmp, int *count)
{
	handle_command_token(token, j, tmp, count);
	handle_args_and_redir(token, j, tmp, count);
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
	printf(PURPLE "cmd_line: %s" RESET "\n", cmd_line);
	token = ft_split_path(cmd_line, ' ');
	tmp->dir = create_new_dir();
	if (!tmp->dir)
		return (FAILURE);
	head = tmp->dir;
	while (token[j] != NULL)
		if (treat_token(token, &j, tmp, &count))
			return (1);
	tmp->dir = head;
	if (tmp->dir && tmp->dir->file == NULL)
		return (free_string_array(token), 0);
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
	free_string_array(token);
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

t_block	*tokenization(char *input)
{
	char	**split_input;
	char	*tmp_input;
	t_block	*original;
	t_block	*tmp;
	int		i;

	i = 0;
	original = new_block();
	tmp = original;
	tmp_input = ft_strdup(input);
	input = add_spaces(tmp_input);
	free(tmp_input);
	split_input = ft_split_path(input, '|');
	while (split_input[i])
	{
		*tmp = (t_block){NULL, NULL, STDOUT_FILENO, STDIN_FILENO, NULL, NULL};
		if (treat_cmd_line(split_input[i], tmp))
			return (NULL);
		if (split_input[i + 1])
			tmp->next = new_block();
		if (tmp->next)
			tmp = tmp->next;
		i++;
	}
	tmp->next = NULL;
	if (split_input)
		free_string_array(split_input);
	return (original);
}
