/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verifcmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:07:44 by myassine          #+#    #+#             */
/*   Updated: 2023/12/26 21:03:45 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char	*verif_cmd(char **args, char **env)
{
	char	*tmp;
	char	**path;
	char	*str;
	int		i;

	i = -1;
	if (!args || !args[0])
		return (NULL);
	if (is_bultin(args[0]) || access(args[0], F_OK) == 0)
		return (args[0]);
	path = getPath(env);
	tmp = ft_strdup(args[0]);
	free(args[0]);
	args[0] = ft_strjoin("/", tmp);
	free(tmp);
	while (path[++i])
	{
		str = ft_strjoin(path[i], args[0]);
		if (access(str, F_OK) == 0)
			return (freeStringArray(path), free(args[0]), str);
		free(str);
	}
	freeStringArray(path);
	tmp = ft_substr(args[0], 1, ft_strlen(args[0]));
	free(args[0]);
	args[0] = ft_strdup(tmp);
	if (tmp)
		free(tmp);
	return (NULL);
}
