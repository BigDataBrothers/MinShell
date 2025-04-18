/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 02:20:08 by myassine          #+#    #+#             */
/*   Updated: 2024/02/27 16:32:26 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_error(int error)
{
	if (!error)
		return ;
	if (error == 1)
		write(2, "Syntax error :\nSimple quote is open\n", 37);
	else if (error == 2)
		write(2, "Syntax error :\nDouble quote is open\n", 37);
	else if (error == 3)
		write(2, "Syntax error :\nRedirection is fail\n", 36);
	else if (error == 4)
		write(2, "Syntax error :\nPipe is fail\n", 29);
	else if (error == 5)
		write(2, "Syntax error :\nambiguous redirect\n", 35);
}

void	print_env(t_env *env, t_env *head_env)
{
	while (env)
	{
		if (env->str[0] && env->str[1])
			printf("%s=%s\n", env->str[0], env->str[1]);
		env = env->next;
	}
	env = head_env;
}

void	print_input(char *input)
{
	int	i;

	i = -1;
	while (input[++i])
	{
		if (is_neg_char(input[i]))
		{	
			input[i] *= -1;
			printf("%c", input[i]);
		}
		else
			printf("%c", input[i]);
	}
	printf("\n");
}

void	print_tab(char **tab, char *prefix)
{
	int	i;

	i = 0;
	while (tab && tab[i])
	{
		printf("%s = [%s][%d]\n", prefix, tab[i], i);
		i++;
	}
}

void	print_error_2(char *pre_arg, char *arg, char *post_arg)
{
	if (pre_arg)
		write(2, pre_arg, ft_strlen(pre_arg));
	if (arg)
		write(2, arg, ft_strlen(arg));
	if (post_arg)
		write(2, post_arg, ft_strlen(post_arg));
	write(2, "\n", 1);
}
