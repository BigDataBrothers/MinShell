/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quote_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 21:47:40 by myassine          #+#    #+#             */
/*   Updated: 2024/01/12 18:09:01 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_quote(char *input)
{
	int	i;
	int	d;
	int	s;

	i = 0;
	d = 0;
	s = 0;
	while (input && input[i])
	{
		if (is_simple_quote(input[i]) || is_double_quote(input[i]))
		{
			if (is_simple_quote(input[i]))
				s++;
			else if (is_double_quote(input[i]))
				d++;
			if (input[i + 1])
				i = process_quotes(input, i, &s, &d);
		}
		if (input[i])
			i++;
	}
	return (check_quotes(s, d));
}

void	in_quote(char *input)
{
	int	i;
	int	d;
	int	s;

	i = 0;
	d = 0;
	s = 0;
	while (input[i])
	{
		if (is_simple_quote(input[i]) && !d)
			s = handle_simple_quote(s);
		else if (is_double_quote(input[i]) && !s)
			d = handle_double_quote(d);
		else if (d && is_doll_sign(input[i]))
			i = process_dollar_sign(input, i);
		if (((s && !is_simple_quote(input[i])) || (d
					&& !is_double_quote(input[i]))) && !is_neg_char(input[i]))
			input[i] *= -1;
		if (input[i])
			i++;
	}
}

char	*if_quote(char *input)
{
	int		i;
	char	*str;
	char	c;

	i = 0;
	if (!input)
		return (NULL);
	while (input[i])
	{
		if (is_double_quote(input[i]) || is_simple_quote(input[i]))
		{
			c = input[i];
			str = remove_char_at_index(input, i);
			while (str[i] && str[i] != c)
				if (str[i++] < 0)
					str[i - 1] *= -1;
			input = remove_char_at_index(str, i);
			free(str);
		}
		else if (input[i])
			i++;
	}
	return (input);
}

char	*no_quote(char *str)
{
	char	*n_str;
	int		i;
	int		j;

	if (is_simple_quote(str[0]) || is_double_quote(str[0]))
	{
		n_str = NULL;
		n_str = malloc(sizeof(char) * ft_strlen(str) - 1);
		if (!n_str)
			return (NULL);
		i = 1;
		j = 0;
		while (str[i])
		{
			if (i == ft_strlen(str) - 1)
				break ;
			n_str[j++] = str[i++];
		}
		n_str[j] = '\0';
		return (n_str);
	}
	else
		return (str);
}
