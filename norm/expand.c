/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 02:19:09 by myassine          #+#    #+#             */
/*   Updated: 2024/01/25 21:04:06 by myassine         ###   ########.fr       */
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
	ptr = NULL;
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

int	ft_sizeint(int n)
{
	int	i;

	i = 0;
	if (n < 0)
	{
		n *= -1;
		i++;
	}
	while (n > 9)
	{
		n /= 10;
		i++;
	}
	return (i + 1);
}

void	process_exp_status(int *i, t_all *all)
{
	char	*v[6];
	int		len_exp;
	int		j;

	v[pre_exp] = ft_substr(all->input, 0, *i);
	len_exp = len_word_exp(all->input, *i) + 1;
	v[exp] = ft_substr(all->input, *i, len_exp);
	j = *i + ft_strlen(v[exp]);
	v[post_exp] = ft_substr(all->input, j, ft_strlen(all->input));
	v[tmp] = ft_itoa(all->status);
	j = -1;
	while (v[tmp] && v[tmp][++j])
		if (is_spec_char(v[tmp][j]))
			v[tmp][j - 1] *= -1;
	v[post_input] = ft_strjoin(v[pre_exp], v[tmp]);
	v[concat] = ft_strjoin(v[post_input], v[post_exp]);
	if (v[post_input])
	{
		free(v[post_input]);
		v[post_input] = NULL;
	}
	v[post_input] = ft_strdup(v[concat]);	
	all->input = ft_strdup(v[post_input]);
	free_exp(v);
}

void	process_exp_variable(int *i, t_all *all)
{
	char	*unknow[6];
	int		len_exp;
	int		j;
	// char 	*to_free;

	if (all->input[*i] == '$' && all->input[*i + 1] == '?')
	{
		process_exp_status(i, all);
		return ;
	}
	unknow[pre_exp] = ft_substr(all->input, 0, *i);
	len_exp = len_word_exp(all->input, *i);
	unknow[exp] = ft_substr(all->input, *i, len_exp);
	j = *i + ft_strlen(unknow[exp]);
	unknow[post_exp] = ft_substr(all->input, j, ft_strlen(all->input));
	unknow[tmp] = exp_(unknow[exp], all->env, all->head_env);
	j = -1;
	while (unknow[tmp] && unknow[tmp][++j])
		if (is_spec_char(unknow[tmp][j]))
			unknow[tmp][j - 1] *= -1;
	unknow[post_input] = ft_strjoin(unknow[pre_exp], unknow[tmp]);
	unknow[concat] = ft_strjoin(unknow[post_input], unknow[post_exp]);
	if (unknow[post_input])
		free(unknow[post_input]);
	unknow[post_input] = ft_strdup(unknow[concat]);
	// if (all->input)
	// 	free(all->input);
	// to_free = all->input;
	if (all->input)
	{
		free(all->input);
		all->input = NULL;
	}
	all->input = unknow[post_input]; // all->input = ft_strdup(unknow[post_input]);
	// if(all->input != to_free && to_free)
		// free(to_free);
	free_exp(unknow);
}


void	expa_chang(t_all *all)
{
	int		i;
	
	i = 0;
	while (all->input[i] && all->input[i + 1])
	{
		if (is_doll_sign(all->input[i]))
		{
			process_exp_variable(&i, all);
		}
		if (is_doll_sign(all->input[i]) && all->input[i + 1] && is_alpha(all->input[i + 1]))
			i--;
		else if (all->input[i])
			i++;
	}
}
