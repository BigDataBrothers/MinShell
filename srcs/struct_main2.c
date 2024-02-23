/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_main2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 17:39:28 by myassine          #+#    #+#             */
/*   Updated: 2024/02/16 01:03:03 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_all(t_all *all, char **envp)
{
	all->env = init_env(all->env, all->head_env, envp);
	all->pipe_fds[0] = -1;
	all->pipe_fds[1] = -1;
	all->pid = -1;
}

char	*ft_strdupf(char *s)
{
	int		len;
	int		i;
	char	*d;

	i = 0;
	if (s == NULL)
		return (NULL);
	len = ft_strlen(s);
	d = malloc(sizeof(char) * (len + 1));
	if (!d)
		return (NULL);
	while (s[i] != '\0')
	{
		d[i] = s[i];
		i++;
	}
	d[i] = '\0';
	free(s);
	return (d);
}

char	*ft_strjoin_lfree(char *s1, char *s2)
{
	int		len1;
	int		len2;
	char	*s3;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	s3 = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!s3)
		return (NULL);
	ft_strcpy(s3, s1);
	ft_strcpy(s3 + len1, s2);
	return (free(s1), s3);
}

char	*ft_strjoin_rfree(char *s1, char *s2)
{
	int		len1;
	int		len2;
	char	*s3;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	s3 = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!s3)
		return (NULL);
	ft_strcpy(s3, s1);
	ft_strcpy(s3 + len1, s2);
	return (free(s2), s3);
}

char	*ft_strjoin_dfree(char *s1, char *s2)
{
	int		len1;
	int		len2;
	char	*s3;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	s3 = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!s3)
		return (NULL);
	ft_strcpy(s3, s1);
	ft_strcpy(s3 + len1, s2);
	return (free(s1), free(s2), s3);
}
