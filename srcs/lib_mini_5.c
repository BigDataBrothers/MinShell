/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_mini_5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 16:23:48 by myassine          #+#    #+#             */
/*   Updated: 2024/02/25 16:33:19 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	nbr_arg(char **token, int j)
{
	int	i;
	int	len;

	i = 0;
	len = j;
	while (token[j])
	{
		if (is_redir(token[j]))
		{
			if (!token[j + 1])
				return (-1);
			j += 2;
			i += 2;
		}
		else
			j++;
	}
	j -= len;
	j -= i;
	return (j);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

int	is_space_word(char *str)
{
	int	i;

	i = -1;
	if (!str)
		return (0);
	while (str[++i])
		if (!is_space(str[i]))
			return (1);
	return (0);
}

int	is_empty_input(t_all *all)
{
	int	i;

	i = -1;
	while (all->input && all->input[++i])
	{
		if (!is_space(all->input[i]) && all->input[i] != 9)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	len_z_tab(char *str)
{
	int	len;
	int	it;

	len = 0;
	it = -1;
	while (str[++it])
		if (str[it] != '\t')
			len++;
	return (len);
}
