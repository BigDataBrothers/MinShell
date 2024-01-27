/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 20:54:16 by myassine          #+#    #+#             */
/*   Updated: 2024/01/26 20:44:09 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//si j ai le tps tri l env de export par ordre alpha
int	is_plus(char c)
{
	return (c == '+');
}

int	is_egal(char c)
{
	return (c == '=');
}

void	print_exp(t_env *env, t_env *head_env)
{
	while (env)
	{
		if (env->str[0])
		{
			write(1,"declare -x ", 11);
			write(1, env->str[0], ft_strlen(env->str[0]));
		}
			// printf("declare -x %s", env->str[0]);
		if (env->str[1])
		{
			write(1,"=\"", 2);
			write(1, env->str[1], ft_strlen(env->str[1]));
			write(1,"\"\n", 2);
		}
			// printf("=\"%s\"\n", env->str[1]);
		else if (!env->str[1])
			write(1,"\n", 1);
			// printf("\n");
		env = env->next;
	}
	env = head_env;
}

int	check_export(char *str)
{
	int	i;
	int	sig_egal;

	i = 0;
	sig_egal = 0;
	while (str[i] && (is_alpha(str[i]) || is_underscor(str[i])))
		i++;
	if (i == 0)
		return (0);
	while (str[i] && is_alnum(str[i]))
	{
		if (!is_alnum(str[i]))
			return (0);
		i++;
	}
	if (str[i] && is_egal(str[i]))
	{
		i++;
		sig_egal++;
	}
	if (sig_egal)
		return (2);
	return (1);
}

char	*pre_egal(char *str)
{
	char	*tmp;
	int		i;

	i = 0;
	while (str[i] && !is_egal(str[i]))
		i++;
	tmp = malloc(sizeof(char) * (i + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (str[i] && !is_egal(str[i]))
	{
		tmp[i] = str[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}
