/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 22:59:48 by myassine          #+#    #+#             */
/*   Updated: 2024/03/01 16:54:12 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	str_void(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		skip_whitespace(input, &i);
		if (input[i])
			return (1);
		i++;
	}
	return (0);
}

int	check_error(char *input)
{
	int	error;

	if (!input)
		return (0);
	error = 0;
	error = ft_quote(input);
	if (error)
		return (error);
	error = check_redir(input);
	if (error)
		return (error);
	error = check_pipe(input);
	if (error)
		return (error);
	return (0);
}

int	check_error_1(char *input)
{
	int	error;

	if (!input)
		return (0);
	error = 0;
	error = check_redir(input);
	if (error)
		return (5);
	return (0);
}
