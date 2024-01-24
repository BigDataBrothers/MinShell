/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-tena <ade-tena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:07:15 by myassine          #+#    #+#             */
/*   Updated: 2024/01/20 09:27:55 by ade-tena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_current_directory_with_prompt(void)
{
	static char	cwd[PATH_MAX];

	getcwd(cwd, sizeof(cwd));
	return (cwd);
}

void	eof(char *input, char **envs, t_env *env, t_env *headenv)
{
	terminat(input, envs, env, headenv);
}

char	**get_path(char **env)
{
	int		i;
	char	**path;

	i = 0;
	path = NULL;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0 && ft_strlen(env[i]) > 5)
		{
			path = ft_split_path(env[i], ':');
			break ;
		}
		i++;
	}
	return (path);
}

int	no_input(char *input)
{
	int	i;

	i = 0;
	if (!input || input[i] == '\0')
		return (0);
	skip_whitespace(input, &i);
	if (!input || input[i] == '\0')
		return (0);
	return (1);
}
