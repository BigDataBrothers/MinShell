/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verifcmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:07:44 by myassine          #+#    #+#             */
/*   Updated: 2024/02/23 17:18:07 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*verif_cmd_2(char **path, char *tmp, char **env, char **args)
{
	int	i;

	i = -1;
	path = get_path(env);
	tmp = ft_strdup(args[0]);
	args[0] = ft_strjoin_rfree("/", tmp);
	if (path != NULL)
	{
		while (path[++i])
		{
			tmp = ft_strjoin(path[i], args[0]);
			if (access(tmp, F_OK) == 0)
				return (free_string_array(path), free(args[0]), tmp);
			free(tmp);
		}
	}
	return (NULL);
}

char	*verif_cmd(char **args, char **env)
{
	char	*tmp;
	char	**path;
	int		i;

	i = -1;
	if (!args || !args[0] || is_bultin(args[0]))
		return (NULL);
	path = get_path(env);
	tmp = mouv_tab(args[0]);
	if (ft_strchr(tmp, '/'))
	{
		if (access(tmp, F_OK | X_OK) == 0)
			return (free_string_array(path), free(args[0]), args[0] = NULL, tmp);
		return (free_string_array(path) ,free(tmp), free(args[0]), args[0] = NULL, NULL);
	}
	free(args[0]);
	args[0] = ft_strjoin_rfree("/", tmp);
	if (path != NULL)
	{
		while (path[++i])
		{
			tmp = ft_strjoin(path[i], args[0]);
			if (access(tmp, F_OK) == 0)
				return (free_string_array(path), free(args[0]), tmp);
			free(tmp);
		}
	}
	free_string_array(path);
	tmp = ft_substr(args[0], 1, ft_strlen(args[0]));
	return (free(args[0]), args[0] = ft_strdup(tmp), if_free(tmp), NULL);
}
