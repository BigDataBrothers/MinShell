/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verifcmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:07:44 by myassine          #+#    #+#             */
/*   Updated: 2024/02/25 19:13:52 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*while_verif(char **tmp, char **path, char **args)
{
	int	i;

	i = -1;
	while (path[++i])
	{
		(*tmp) = ft_strjoin(path[i], args[0]);
		if (access((*tmp), F_OK) == 0)
		{
			free_string_array(path);
			free(args[0]);
			args[0] = NULL;
			return ((*tmp));
		}
		free((*tmp));
	}
	return (NULL);
}

char	*verif_cmd(char **args, char **env)
{
	char	*tmp;
	char	**path;

	if (!args || !args[0] || ft_strncmp(args[0], "./", 2) == 0)
		return (NULL);
	if (is_bultin(args[0]) || access(args[0], F_OK) == 0)
	{
		tmp = ft_strdup(args[0]);
		if (!tmp)
			return (NULL);
		return (tmp);
	}
	path = get_path(env);
	tmp = ft_strdupf(args[0]);
	args[0] = ft_strjoin_rfree("/", tmp);
	if (path != NULL)
	{
		tmp = while_verif(&tmp, path, args);
		if (tmp)
			return (tmp);
		free(tmp);
	}
	free_string_array(path);
	tmp = ft_substr(args[0], 1, ft_strlen(args[0]));
	return (free(args[0]), args[0] = ft_strdup(tmp), if_free(tmp), NULL);
}
