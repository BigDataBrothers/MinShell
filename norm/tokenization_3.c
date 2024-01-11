/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 18:44:48 by myassine          #+#    #+#             */
/*   Updated: 2024/01/11 18:33:15 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	return (original);
}

void	cleanup_memory(char **split_input)
{
	if (split_input)
		free_string_array(split_input);
}

t_block	*tokenization(char *input)
{
	char	**split_input;
	t_block	*original;

	original = initialize_and_prepare(&input);
	split_input = ft_split_path(input, '|');
	process_commands(split_input, original);
	cleanup_memory(split_input);
	return (original);
}
