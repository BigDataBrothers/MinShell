/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:07:05 by myassine          #+#    #+#             */
/*   Updated: 2024/02/23 17:06:38 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*ft_cd(char **args, t_env *env, t_env *head_env, t_all *all)
{
	int		res;
	char	*home;

	if (args[1] && args[2])
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		return (all->status = 1, NULL);
	}
	if (!args[1])
	{
		home = ft_get_env("HOME", env, head_env);
		if (!home)
		{
			write(2, "$HOME not set\n", 14);
			return (all->status = 1, NULL);
		}
		res = chdir(home);
		if (res == -1)
		{
			perror("cd");
			return (all->status = 1, NULL);
		}
		return (all->status = 0, NULL);
	}
	res = chdir(args[1]);
	if (res == -1)
	{
		perror("cd");
		return (all->status = 1, NULL);
	}
	return (all->status = 0, NULL);
}

int	aplic_bulltin(t_all *all, t_env *env, t_env *head_env, char **args)
{
	if (args && !ft_strcmp(all->test->cmd, "cd"))
		return (ft_cd(args, env, head_env, all), \
		free(all->test->cmd), all->test->cmd = NULL, 1);
	else if (args && !ft_strcmp(all->test->cmd, "unset"))
		return (ft_unset(env, head_env, all->test), \
		free(all->test->cmd), all->test->cmd = NULL, 1);
	else if (args && !ft_strcmp(all->test->cmd, "export"))
		return (ft_export(env, head_env, all->test), \
		free(all->test->cmd), all->test->cmd = NULL, 1);
	else if (args && !ft_strcmp(all->test->cmd, "pwd"))
		return (ft_pwd(all->test), \
		free(all->test->cmd), all->test->cmd = NULL, 1);
	else if (args && !ft_strcmp(all->test->cmd, "echo"))
		return (ft_echo(all->test), \
		free(all->test->cmd), all->test->cmd = NULL, 1);
	else if (args && !ft_strcmp(all->test->cmd, "env"))
		return (ft_env(env, head_env, all->test), \
		free(all->test->cmd), all->test->cmd = NULL, 1);
	return (0);
}
