/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:34:30 by myassine          #+#    #+#             */
/*   Updated: 2024/03/01 16:20:31 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	rpl_en_var_2(char *str, t_env *tmp)
{
	char	*te;

	if (tmp->str[1] != NULL)
		free(tmp->str[1]);
	te = if_quote(str);
	tmp->str[1] = post_egal(te);
	if (!tmp->str[1])
		return (0);
	return (1);
}

t_env	*rpl_env_var(t_env *env, t_env *head_env, char *str)
{
	t_env	*tmp;
	char	*p_e;

	while (env)
	{
		tmp = env->next;
		p_e = pre_egal(str);
		if (!p_e)
			return (NULL);
		if (env->next && !ft_strcmp(env->next->str[0], p_e))
		{
			free(p_e);
			if (tmp)
				if (!rpl_en_var_2(str, tmp))
					return (NULL);
			return (env);
		}
		free(p_e);
		env = env->next;
	}
	return (env = head_env, env);
}

t_env	*sup_env_var(t_env *env, t_env *head_env, char *str)
{
	t_env	*tmp;
	t_env	*tmp_av;
	char	*str2;

	str2 = pre_egal(str);
	tmp = env;
	tmp_av = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->str[0], str2))
		{
			if (tmp_av)
				tmp_av->next = tmp->next;
			if (tmp == env)
				env = tmp->next;
			free(tmp->str[0]);
			free(tmp->str[1]);
			free(tmp);
			free(str2);
			return (env);
		}
		tmp_av = tmp;
		tmp = tmp->next;
	}
	return (free(str2), head_env);
}

void	ft_env(t_env *env, t_env *head_env, t_block *block)
{
	if (!ft_strcmp(block->cmd, "env") && !block->arg)
		print_env(env, head_env);
}

int	cut_export_argument(char *arg_export)
{
	char	**exported;
	int		status_export;

	exported = ft_split_path(arg_export, '=');
	status_export = check_export(exported[0]);
	if (!status_export)
		return (print_error_2("export: `", exported[0],
				"': not a valid identifier\n"), SUCCESS);
	return (SUCCESS);
}
