/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 21:39:12 by myassine          #+#    #+#             */
/*   Updated: 2023/12/08 20:30:07 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int treat_token(char **token, int *j, t_block *tmp, int *count) //token[*j]
{
	// if (is_redir(token[*j])) // REDIR ========================
	// {
	// 	tmp->dir = new_dir(token[*j], token[*j + 1], tmp->dir);
	// 	if (!token[*j + 1])
	// 		return (1);
	// 	(*j)+=2;
	// }
	// else					// ARG ===========================
	// {
	//  int *count
	//	token[*j]

		if ((*count) == 0 && !is_redir(token[*j]))
		{
			tmp->cmd = ft_strdup(token[*j]);//
			(*count)++;
			(*j)++;
		}
		else //if (*count)
		{
			int v = 0;
			int argc = 0;
			argc = nbr_arg(token, (*j), count);
			if (argc == -1)
				return (1);
			printf("argc == %d \n", argc);
			char **args = (char **)malloc(sizeof(char *) * (argc + 1));
			if (!args)
				return (1);
			while (token[*j])
			{
				if(is_redir(token[*j]))
				{
					tmp->dir = new_dir(token[*j], token[*j + 1], tmp->dir);
					if (!token[*j + 1])
						return (1);
					(*j)+=2;
				}
				else if(*count == 0)
				{
					tmp->cmd = ft_strdup(token[*j]);//
					(*count)++;
					(*j)++;
				}
				else
				{
					args[v] = ft_strdup(token[*j]);
					v++;
					(*j)++;
				}
			}
			args[v] = NULL;
			tmp->arg = args;
		}

			
			/* 	
			while(token[*j])
			{
				printf("token[%d] : %s\n", j, token[j]);
				if(is_redir(token[j]))
				{
					if (!token[j + 1])
						return (-1);
					printf("redir oh : %s\n", token[j]);
					j+=2;
					i+=2;
					
					// return (j);
				}
				else
					j++;
	*/

		// }
	// }
	// if (token[*j])
	// 	(*j)++;
	return (0);
}
int	treat_cmd_line(char *cmd_line, t_block *tmp)
{
	char	**token;
	int		j;
	int		count;
	count = 0;
	
	j = 0;

	token = ft_split_path(cmd_line, ' ');
	tmp->dir = NULL;
	while (token[j] != NULL) // MOTS ============================
	{
		if (treat_token(token, &j, tmp, &count))
			return (1);
	}
	freeStringArray(token);
	return (0);
}

t_block *tokenization(char *input)
{
	t_block *original;
	t_block *tmp;


	int		i;

	original = new_block();
	tmp = original;
	i = 0;
	char	**split_input;
	split_input = ft_split_path(input, '|');
	while (split_input[i]) 
	{
		if (treat_cmd_line(split_input[i],tmp)) // PIPELINE
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