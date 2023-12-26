/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 21:16:50 by myassine          #+#    #+#             */
/*   Updated: 2023/12/26 21:03:00 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	build_env_2(t_env *env, t_env *head_env, char **envp)
{
	envp = envp;
	head_env = env;
	env->str[0] = "PWD";
	env->str[1] = get_current_directory_with_prompt();
	env->next = malloc(sizeof(t_env));
	if (!env->next)
		return (FAILURE);
	env = env->next;
	env->str[0] = "SHLVL";
	env->str[1] = "1";
	env->next = malloc(sizeof(t_env));
	if (!env->next)
		return (FAILURE);
	env = env->next;
	env->str[0] = "_";
	env->str[1] = "/usr/bin/env";
	env->next = NULL;
	env = head_env;
	return (SUCCESS);
}

int	get_env(t_env *env, t_env *head_env, char **envp)
{
	if (!envp[0])
	{
		if (!build_env_2(env, head_env, envp))
			return (FAILURE);
	}
	else
	{
		if (!build_env(env, head_env, envp))
			return (FAILURE);
	}
	return (SUCCESS);
}

t_env	*init_env(t_env *env, t_env *head_env, char **envp)
{
	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	if (!get_env(env, head_env, envp))
		return (free_env(env, head_env), NULL);
	head_env = env;
	return (env);
}

void	copy_string(char **destination, char *source, int start, int end)
{
	*destination = malloc(sizeof(char) * (end - start + 1));
	if (!*destination)
		return ;
	ft_strcpy_len(&(*destination), source, start, end);
}

int	build_env(t_env *env, t_env *head_env, char **envp)
{
	int	i;
	int	key_len;

	i = 0;
	head_env = env;
	while (envp[i])
	{
		key_len = ft_strlen_target(envp[i], '=');
		copy_string(&(env->str[0]), envp[i], 0, key_len);
		copy_string(&(env->str[1]), envp[i], key_len + 1, ft_strlen(envp[i]));
		if (!env->str[0] || !env->str[1])
			return (FAILURE);
		if (!envp[i + 1])
		{
			env->next = NULL;
			env = head_env;
			break ;
		}
		env->next = malloc(sizeof(t_env));
		if (!env->next)
			return (FAILURE);
		env = env->next;
		i++;
	}
	return (SUCCESS);
}
