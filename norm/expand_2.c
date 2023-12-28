/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 19:57:27 by myassine          #+#    #+#             */
/*   Updated: 2023/12/28 20:02:43 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*exp_var(char *input, int *i)
{
	int		j;
	int		k;

	while (input[*i])
	{
		if (input[*i + 1] && is_doll_sign(input[*i]) && is_alpha(input[*i + 1]))
		{
			j = *i;
			while ((input[++j]) && (is_alnum(input[j])))
				;
			if (j == *i + 1)
				return ("$");
			k = *i - j;
			return (ft_substr(&input[*i], *i, k));
		}
		else if (is_doll_sign(input[*i]) && ((!input[*i + 1]
					|| !is_alpha(input[*i + 1]))))
			return ("$");
		if (input[*i])
			(*i)++;
	}
	return (NULL);
}

int	len_word_exp(const char *str, int i)
{
	int	length;

	length = 1;
	if (str[i + 1])
		i++;
	else
		return (1);
	if (str[i] && (is_num(str[i])))
		return (2);
	else if (str[i] && (is_alpha(str[i]) || is_underscor(str[i])))
	{
		while (str[i] && (is_alnum(str[i]) || is_underscor(str[i])))
		{
			i++;
			length++;
		}
	}
	return (length);
}

char	*ft_get_env(char *exp, t_env *env, t_env *head_env)
{
	char	*tmp;

	tmp = NULL;
	while (env)
	{
		if (!ft_strcmp(exp, env->str[0]))
		{
			tmp = ft_strdup(env->str[1]);
			env = head_env;
			return (tmp);
		}
		env = env->next;
	}
	env = head_env;
	if_free(tmp);
	return (NULL);
}
