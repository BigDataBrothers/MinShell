/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:07:05 by myassine          #+#    #+#             */
/*   Updated: 2024/01/24 17:58:08 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_cd(t_all *all)
{
	char *tmp;
	char *tmp2;

	// printf("\n\n%s\n\n", all->args[1]);
	
	if (all->args && all->args[1] && !ft_strcmp(all->test->cmd, "cd"))
	{
		if (all->args[1][0] == '-')
		{
			tmp = ft_get_env("OLDPWD", all->env, all->head_env);
			chdir(tmp);
			free(tmp);
			tmp = NULL;
			sup_env_var(all->env, all->head_env, "OLDPWD");
			tmp = ft_strjoin("OLDPWD=", ft_substr(all->path, 0, ft_strlen(all->path) - 3));
			export_env_2(all->env, all->head_env, tmp);
			free(tmp);
		}
		else
		{
			sup_env_var(all->env, all->head_env, "OLDPWD");
			tmp2 = ft_substr(all->path, 0, ft_strlen(all->path) - 3);
			tmp = ft_strjoin("OLDPWD=", tmp2);
			free(tmp2);
			export_env_2(all->env, all->head_env, tmp);
			free(tmp);
			if (all->args[1][0] == '~')
			{
				tmp2 = ft_get_env("HOME", all->env, all->head_env);
				tmp = ft_strjoin(tmp2,&all->args[1][1]);
				free(tmp2);
				all->args[1] = tmp;
				free(tmp);
				return (1);
			}
			if (chdir(all->args[1]) == -1)
			{
				printf("cd: %s: %s\n", all->args[1], strerror(errno));
				all->status = 1;
			}
		}
		return (1);
	}
	else if (all->args && all->args[1] && !ft_strcmp(all->test->cmd, "cd") && !all->args[1]
		&& chdir(ft_get_env("HOME", all->env, all->head_env)) == 0)
		return (1);
	else if (all->args && !all->args[1] && !ft_strcmp(all->test->cmd, "cd"))
	{
		tmp = ft_get_env("HOME", all->env, all->head_env);
		if (chdir(tmp) == 1)
		{
			free(tmp);
			return (0);
		}
		free(tmp);
		return (1);
	}
	return (0);	
}

int	applic_bulltin(t_all *all)
{
	if (ft_cd(all))
		return (1);
	else if (all->args && !ft_strcmp(all->test->cmd, "unset"))
		return (ft_unset(all->env, all->head_env, all->test), 1);
	else if (all->args && !ft_strcmp(all->test->cmd, "export"))
		return (ft_export(all->env, all->head_env, all->test, all), 1);
	else if (all->args && !ft_strcmp(all->test->cmd, "pwd"))
		return (ft_pwd(all->test), 1);
	else if (all->args && !ft_strcmp(all->test->cmd, "echo"))
		return (ft_echo(all->test), 1);
	else if (all->args && !ft_strcmp(all->test->cmd, "env"))
		return (ft_env(all->env, all->head_env, all->test), 1);
	return (0);
}
