/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:44:09 by myassine          #+#    #+#             */
/*   Updated: 2023/12/10 17:44:23 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static size_t	ft_num_len(long n)
// {
// 	int	i;

// 	i = 0;
// 	if (n <= 0)
// 		i++;
// 	while (n != 0)
// 	{
// 		n = n / 10;
// 		i++;
// 	}
// 	return (i);
// }

int	process_quotes(char *input, int i, int *s, int *d)
{
	while (input[++i])
	{
		if ((is_simple_quote(input[i]) && *s) \
			|| (is_double_quote(input[i]) && *d))
		{
			*s = 0;
			*d = 0;
			break ;
		}
	}
	return (i);
}

int	handle_simple_quote(int s)
{
	if (s)
		s = 0;
	else
		s = 1;
	return (s);
}

int	handle_double_quote(int d)
{
	if (d)
		d = 0;
	else
		d = 1;
	return (d);
}

int	process_dollar_sign(char *input, int i)
{
	if (input[i + 1] && (is_alpha(input[i + 1]) || \
		is_underscor(input[i + 1])))
		skip_word_exp(input, &i);
	return (i);
}

int	check_quotes(int s, int d)
{
	if (s == 1)
		return (1);
	else if (d == 1)
		return (2);
	return (0);
}

int	ft_quote(char *input)
{
	int	i;
	int	d;
	int	s;

	i = 0;
	d = 0;
	s = 0;
	while (input[i])
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
		if (((s && !is_simple_quote(input[i])) 
			|| (d && !is_double_quote(input[i])))
			&& !is_neg_char(input[i]))
			input[i] *= -1;
		if (input[i])
			i++;
	}
}

char *if_quote(char *input)
{
	int i = 0;
	char c = 0;
	if(input)
	{
		while (input[i])
		{
			if(is_double_quote(input[i]) || is_simple_quote(input[i]))
			{
				c = input[i];
				char *str = NULL;
				str = removeCharAtIndex(input, i);
				while(str[i] != c)
					i++;
				input = removeCharAtIndex(str, i);
				free(str);
			}
			if(input[i])
				i++;	
		}
	}
	return (input);
}

char	*no_quote(char *str)
{
	if(is_simple_quote(str[0]) || is_double_quote(str[0]))
	{
		char *n_str = NULL;
		n_str = malloc(sizeof(char) * ft_strlen(str) - 1);
		if(!n_str)
			return (NULL);
		int i = 1;
		int j = 0;
		while(str[i])
		{
			if(i == ft_strlen(str) - 1)
				break;
			n_str[j++] = str[i++];
		}
		n_str[j] = '\0';
		return(n_str);
	}
	else
		return (str);
}