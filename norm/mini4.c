/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:07:05 by myassine          #+#    #+#             */
/*   Updated: 2024/01/12 19:33:58 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	applic_bulltin(t_block *test, t_env *env, t_env *head_env, char **args)
{
	if (args && args[1] && !ft_strcmp(test->cmd, "cd") && chdir(args[1]) == 0)
		return (1);
	else if (args && args[1] && !ft_strcmp(test->cmd, "cd") && !args[1]
		&& chdir(ft_get_env("HOME", env, head_env)) == 0)
		return (1);
	else if (args && !ft_strcmp(test->cmd, "unset"))
		return (ft_unset(env, head_env, test), 1);
	else if (args && !ft_strcmp(test->cmd, "export"))
		return (ft_export(env, head_env, test), 1);
	else if (args && !ft_strcmp(test->cmd, "pwd"))
		return (ft_pwd(test), 1);
	else if (args && !ft_strcmp(test->cmd, "echo"))
		return (ft_echo(test), 1);
	else if (args && !ft_strcmp(test->cmd, "env"))
		return (ft_env(env, head_env, test), 1);
	return (0);
}
