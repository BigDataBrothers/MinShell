/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir_a_pipe.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:46:28 by myassine          #+#    #+#             */
/*   Updated: 2024/03/01 17:12:24 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_redir_result(int tmp)
{
	if (tmp == 1)
		return (3);
	return (0);
}

int	process_redir(char *input, int *i, int *tmp, int (*is_re_func)(char))
{
	*tmp = 1;
	if (input[*i + 1] && is_re_func(input[*i + 1]))
		(*i)++;
	while (input[++*i])
	{
		if (is_alnum(input[*i]) || is_simple_quote(input[*i]) \
			|| is_double_quote(input[*i]))
			return (*tmp = 0, 0);
		else if (is_spec_char(input[*i]))
		{
			if (is_pipe(input[*i]))
				return (4);
			return (3);
		}
	}
	return (3);
}

int	check_redir(char *input)
{
	int	i;
	int	error;
	int	tmp;

	i = -1;
	error = 0;
	tmp = 0;
	i = 0;
	while (input[i])
	{
		tmp = 0;
		if (is_re_left(input[i]))
			error = process_redir(&(*input), &i, &tmp, is_re_left);
		else if (is_re_right(input[i]))
			error = process_redir(&(*input), &i, &tmp, is_re_right);
		if (error)
			return (error);
		if (input[i])
			i++;
	}
	return (check_redir_result(tmp));
}

int	check_pipe(char *input)
{
	int	i;

	i = 0;
	if (is_space(input[i]))
		skip_whitespace(input, &i);
	if (is_pipe(input[i]))
		return (4);
	while (input[i])
	{
		if (is_pipe(input[i]))
		{
			i++;
			if (!input[i])
				return (4);
			skip_whitespace(input, &i);
			if (is_pipe(input[i]))
				return (4);
		}
		if (input[i])
			i++;
	}
	return (0);
}

int	count_redir(char **token, int j, char *redir)
{
	int	count;

	count = 0;
	while (token[j])
	{
		if (ft_strcmp(token[j], redir))
		{
			count++;
			j++;
		}
		if (token[j])
			j++;
	}
	return (count);
}
