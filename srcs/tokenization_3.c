/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 18:44:48 by myassine          #+#    #+#             */
/*   Updated: 2024/03/01 17:11:12 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*add_spaces(char *str)
{
	char	*result;
	int		i;
	int		j;

	if (ft_strlen(str) == 1)
		result = (char *)malloc(sizeof(char) * (2 * 2 + 1));
	else
		result = (char *)malloc(sizeof(char) * (ft_strlen(str) * 2 + 1));
	if (result == NULL)
		return (NULL);
	i = 0;
	j = i;
	while (str[i] != '\0')
		add_to_special(result, str, &i, &j);
	result[j] = '\0';
	return (result);
}

void	cleanup_memory(char **split_input)
{
	if (split_input)
		free_string_array(split_input);
}

t_block	*tokenization(t_all *all)
{
	char	**split_input;
	char	*spaced_input;
	t_block	*original;

	original = new_block();
	spaced_input = add_spaces(all->input);
	free(all->input);
	all->input = NULL;
	all->input = ft_strdupf(spaced_input);
	split_input = ft_split_path(all->input, '|');
	process_commands(split_input, original);
	free_string_array(split_input);
	return (original);
}
