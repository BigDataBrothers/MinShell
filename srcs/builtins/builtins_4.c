/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:36:06 by myassine          #+#    #+#             */
/*   Updated: 2024/03/01 15:48:39 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_export(t_env *env, t_env *head_env, t_block *block)
{
	int		i;

	i = 0;
	if (block->arg == NULL)
		return (print_exp(env, head_env));
	while (block->arg[i])
	{
		if (!check_export(block->arg[i]))
			print_error_2("export: `", block->arg[i],
				"': not a valid identifier\n");
		else if (check_export(block->arg[i]) == 1
			&& !check_export_exist(env, head_env, block->arg[i]))
			env = export_env_1(env, head_env, block->arg[i]);
		else if (check_export(block->arg[i]) == 1
			&& check_export_exist(env, head_env, block->arg[i]))
			i = i;
		else if (check_export(block->arg[i]) == 2
			&& !check_export_exist(env, head_env, block->arg[i]))
				env = export_env_2(env, head_env, block->arg[i]);
		else if (check_export(block->arg[i]) == 2
			&& check_export_exist(env, head_env, block->arg[i]))
			env = rpl_env_var(env, head_env, block->arg[i]);
		i++;
	}
}

t_all	*ft_unset(t_env *env, t_env *head_env, t_all *all)
{
	if ((!ft_strcmp(all->test->cmd, "unset"))
		&& (all->test->arg && all->test->arg[0])
		&& (check_export_exist(env, head_env, all->test->arg[0])))
		all->env = sup_env_var(env, head_env, all->test->arg[0]);
	return (all);
}

int	is_quote(char *str)
{
	if (str[0] == '\'')
		return (1);
	else if (str[0] == '\"')
		return (2);
	else
		return (0);
}

void	printc(char *input)
{
	char	c;
	int		i;

	i = 0;
	c = 0;
	while (input[i])
	{
		if (input[i] && (input[i] == '\'' || input[i] == '\"'))
		{
			c = input[i];
			i++;
			while (input[i] && input[i] != c)
				printf("%c", input[i++]);
			i++;
		}
		else if (input[i] == 32)
		{
			printf(" ");
			skip_whitespace(input, &i);
		}
		else if (input[i])
			printf("%c", input[i++]);
	}
}

int	check_n(char **tab)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	while (tab[++j] && tab[j][0] == '-' && tab[j][1] && tab[j][1] == 'n')
	{
		while (tab[j][++i])
		{
			if (tab[j][i] != 'n')
				return (1);
		}
	}
	if (j != 1)
		return (2);
	return (0);
}
