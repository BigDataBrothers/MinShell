/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 16:20:50 by myassine          #+#    #+#             */
/*   Updated: 2024/03/01 15:47:49 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	if_free(char *ptr)
{
	if (ptr)
		free(ptr);
}

void	terminat(char *input, char **envs, t_env *env, t_env *head_env)
{
	if (input)
		free(input);
	if (env)
		free_env(env, head_env);
	if (envs)
		free_string_array(envs);
}

void	freeme(t_all *all, int action)
{
	if (all->input)
		free(all->input);
	all->input = NULL;
	if (all->str)
		free(all->str);
	all->str = NULL;
	if (all->args)
		free_string_array(all->args);
	all->args = NULL;
	if (action)
	{
		if (all->envs)
			free_string_array(all->envs);
		if (all->env)
			free_env(all->env, all->head_env);
	}
	if (all->test)
	{
		free_block_list(all->test);
		all->head = NULL;
		all->test = NULL;
	}
}

void	free_struct_all(t_all *all)
{
	if (all->args)
		free_string_array(all->args);
	if (all->env)
		free_env(all->env, all->head_env);
	if (all->envs)
		free_string_array(all->envs);
}

void	all_free_1(t_block *test, t_env *env, t_env *head_env, char **args)
{
	if (test)
		free_block_list(test);
	(void)env;
	(void)head_env;
	(void)args;
}
