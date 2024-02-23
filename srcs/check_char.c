/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:31:35 by myassine          #+#    #+#             */
/*   Updated: 2024/02/02 22:52:09 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_re_right(char c)
{
	return (c == '>');
}

int	is_re_left(char c)
{
	return (c == '<');
}

int	is_pipe(char c)
{
	return (c == '|');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\a' \
		|| c == '\b' || c == '\v' || c == '\f' || c == '\r');
}

int	is_spec_char(char c)
{
	return (c == '<' || c == '>' || c == '|');
}
