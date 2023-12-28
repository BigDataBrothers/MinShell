/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verifcmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:07:44 by myassine          #+#    #+#             */
/*   Updated: 2023/12/28 21:56:27 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*verif_cmd(char **args, char **env)
{
	char	*tmp;
	char	**path;
	int		i;

	i = -1;
	if (!args || !args[0])
		return (NULL);
	if (is_bultin(args[0]) || access(args[0], F_OK) == 0)
		return (args[0]);
	path = get_path(env);
	tmp = ft_strdup(args[0]);
	free(args[0]);
	args[0] = ft_strjoin("/", tmp);
	free(tmp);
	while (path[++i])
	{
		tmp = ft_strjoin(path[i], args[0]);
		if (access(tmp, F_OK) == 0)
			return (free_string_array(path), free(args[0]), tmp);
		free(tmp);
	}
	free_string_array(path);
	tmp = ft_substr(args[0], 1, ft_strlen(args[0]));
	free(args[0]);
	return (args[0] = ft_strdup(tmp), if_free(tmp), NULL);
}
