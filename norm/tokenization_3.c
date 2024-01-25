/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 18:44:48 by myassine          #+#    #+#             */
/*   Updated: 2024/01/25 17:53:03 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*add_spaces(char *str)
{
	char	*result;
	int		i;
	int		j;

	result = (char *)malloc(sizeof(char) * (ft_strlen(str) * 2 + 1));
	if (result == NULL)
		return (NULL);
	i = 0;
	j = i;
	while (str[i])
		add_to_special(result, str, &i, &j);
	result[j] = '\0';
	return (result);
}

t_block	*initialize_and_prepare(char **input)
{
	t_block	*original;
	char	*tmp_input;

	original = new_block();
	tmp_input = ft_strdup(*input);
	*input = add_spaces(tmp_input);
	free(tmp_input);
	tmp_input = NULL;
	return (original);
}

void	cleanup_memory(char **split_input)
{
	if (split_input)
		free_string_array(split_input);
}

void tokenization(t_all *all)
{
	char	**split_input;
	t_block	*original;

	/*if(all->input != NULL)
	{
		free(all->input);
		all->input = NULL;
	}*/
	original = initialize_and_prepare(&all->input);
	split_input = ft_split_path(all->input, '|');
	process_commands(split_input, original);
	cleanup_memory(split_input);
	free_block_list(all->test);
	all->test = original;
	all->test_head = original;
}
