/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:26:50 by myassine          #+#    #+#             */
/*   Updated: 2024/02/28 17:07:02 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	malloc_z(char *tmp)
{
	tmp = malloc(sizeof(char));
	if (!tmp)
		return ;
	tmp[0] = '\0';
}

char	*post_egal(char *str)
{
	char	*tmp;
	int		i;
	int		j;

	tmp = NULL;
	i = 0;
	j = 0;
	while (str[i] && !is_egal(str[i]))
		i++;
	if (!str[i + 1])
	{
		tmp = malloc(sizeof(char));
		if (!tmp)
			return (NULL);
		tmp[0] = '\0';
		return (tmp);
	}
	i++;
	tmp = malloc(sizeof(char) * (ft_strlen(str) - i + 1));
	if (!tmp)
		return (NULL);
	while (str[i])
		tmp[j++] = str[i++];
	tmp[j] = '\0';
	return (tmp);
}

int	check_export_exist(t_env *env, t_env *head_env, char *str)
{
	char	*pre_e;

	pre_e = pre_egal(str);
	if (!pre_e)
		return (0);
	while (env)
	{
		if (!ft_strcmp(env->str[0], pre_e))
		{
			free(pre_e);
			env = head_env;
			return (1);
		}
		env = env->next;
	}
	env = head_env;
	free(pre_e);
	return (0);
}

t_env	*export_env_1(t_env *env, t_env *head_env, char *str)
{
	t_env	*new_block_env;

	new_block_env = malloc(sizeof(t_env));
	if (!new_block_env)
		return (NULL);
	new_block_env->str[0] = ft_strdup(str);
	if (!new_block_env->str[0])
		return (NULL);
	new_block_env->str[1] = NULL;
	new_block_env->next = NULL;
	while (env->next != NULL)
		env = env->next;
	env->next = new_block_env;
	env = head_env;
	return (env);
}

t_env	*export_env_2(t_env *env, t_env *head_env, char *str)
{
	t_env	*new_block_env;

	str = if_quote(str);
	new_block_env = malloc(sizeof(t_env));
	if (!new_block_env)
		return (NULL);
	new_block_env->str[0] = pre_egal(str);
	if (!new_block_env->str[0])
		return (NULL);
	new_block_env->str[1] = post_egal(str);
	new_block_env->next = NULL;
	if (env)
	{
		while (env->next != NULL)
			env = env->next;
		env->next = new_block_env;
	}
	env = head_env;
	return (env);
}
