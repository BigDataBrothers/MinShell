/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:44:09 by myassine          #+#    #+#             */
/*   Updated: 2023/12/28 21:51:23 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	process_quotes(char *input, int i, int *s, int *d)
{
	while (input[++i])
	{
		if ((is_simple_quote(input[i]) && *s) || (is_double_quote(input[i])
				&& *d))
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
	if (input[i + 1] && (is_alpha(input[i + 1]) || is_underscor(input[i + 1])))
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
