/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 02:19:09 by myassine          #+#    #+#             */
/*   Updated: 2024/01/12 17:45:40 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

enum e_vars
{
	pre_exp,
	exp,
	post_exp,
	tmp,
	post_input,
	concat
};

void	if_free(char *ptr)
{
	if (ptr)
		free(ptr);
}

char	*exp_(char *exp, t_env *env, t_env *head_env)
{
	char	*tmp;

	tmp = NULL;
	if (!exp[1])
		return ("$");
	tmp = ft_get_env(exp + 1, env, head_env);
	if (!tmp)
		return (NULL);
	return (tmp);
}

void	free_exp(char **v)
{
	if_free(v[post_input]);
	if_free(v[concat]);
	if_free(v[post_exp]);
	if_free(v[exp]);
	if_free(v[pre_exp]);
	if_free(v[tmp]);
}

char	*process_exp_variable(char *input, int *i, t_env *env, t_env *head_env)
{
	char	*v[6];
	int		len_exp;
	int		j;

	v[pre_exp] = ft_substr(input, 0, *i);
	len_exp = len_word_exp(input, *i);
	v[exp] = ft_substr(input, *i, len_exp);
	j = *i + ft_strlen(v[exp]);
	v[post_exp] = ft_substr(input, j, ft_strlen(input));
	v[tmp] = exp_(v[exp], env, head_env);
	j = -1;
	while (v[tmp] && v[tmp][++j])
		if (is_spec_char(v[tmp][j]))
			v[tmp][j - 1] *= -1;
	v[post_input] = ft_strjoin(v[pre_exp], v[tmp]);
	v[concat] = ft_strjoin(v[post_input], v[post_exp]);
	if (v[post_input])
		free(v[post_input]);
	v[post_input] = strdup(v[concat]);
	if (input)
		free(input);
	input = strdup(v[post_input]);
	return (free_exp(v), input);
}
//========================================================

char	*expa_chang(char *input, t_env *env, t_env *head_env)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	while (input[i] && input[i + 1])
	{
		if (is_doll_sign(input[i]))
		{
			tmp = process_exp_variable(input, &i, env, head_env);
			if (!tmp)
			{
				if_free(input);
				break ;
			}
			input = ft_strdup(tmp);
			if_free(tmp);
		}
		if (is_doll_sign(input[i]) && input[i + 1] && is_alpha(input[i + 1]))
			i--;
		else if (input[i])
			i++;
	}
	return (input);
}
