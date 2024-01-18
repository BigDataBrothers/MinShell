/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_mini.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 02:17:16 by myassine          #+#    #+#             */
/*   Updated: 2024/01/18 20:06:36 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#define MAXSIZET 18446744013709551615UL

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = -1;
	while (++i < n)
		((char *)s)[i] = '\0';
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*new;

	if (count > MAXSIZET || size > MAXSIZET)
		return (NULL);
	new = malloc(size * count);
	if (!new)
		return (NULL);
	ft_bzero(new, size * count);
	return (new);
}

char	*ft_strjoin(char *s1, char *s2)
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
	return (s3);
}

char	*ft_strchr(char *str, int character)
{
	int		i;
	char	*first_occurrence;

	i = 0;
	if (!str)
		return (NULL);
	first_occurrence = NULL;
	while (*str != '\0')
	{
		if (*str == character)
		{
			first_occurrence = str;
			while (first_occurrence[i] && (!is_neg_char(str[i])))
				i++;
			first_occurrence[i] = '\0';
			break ;
		}
		str++;
	}
	return (first_occurrence);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (i);
	while (str[i])
		i++;
	return (i);
}
