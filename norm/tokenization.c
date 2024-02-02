/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 21:39:12 by myassine          #+#    #+#             */
/*   Updated: 2024/02/02 23:43:58 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_dir	*create_new_dir(void)
{
	t_dir	*new_dir;

	new_dir = malloc(sizeof(t_dir));
	if (!new_dir)
		return (FAILURE);
	new_dir->file = NULL;
	new_dir->fd = -1;
	new_dir->type = -1;
	new_dir->next = NULL;
	return (new_dir);
}

void	process_commands(char **split_input, t_block *original)
{
	t_block	*tmp;
	int		i;

	tmp = original;
	i = 0;
	while (split_input[i])
	{
		*tmp = (t_block){NULL, NULL, STDOUT_FILENO, STDIN_FILENO, NULL, NULL};
		if (treat_cmd_line(split_input[i], tmp))
			return ;
		if (split_input[i + 1])
			tmp->next = new_block();
		if (tmp->next)
			tmp = tmp->next;
		i++;
	}
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
void	process_command_token(char **token, int *j, t_block *tmp)
{
	if (!tmp->cmd && !is_redir(token[*j]))
	{		
		tmp->cmd = ft_strdup(token[(*j)++]);
		// printf(CYAN"tmp->cmd: %p"RESET"\n", tmp->cmd);
	}
}

void	process_redirection_token(char **token, int *j, t_block *tmp)
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
