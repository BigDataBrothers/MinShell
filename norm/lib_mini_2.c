/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_mini_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 20:11:37 by myassine          #+#    #+#             */
/*   Updated: 2023/12/28 20:36:31 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#define MAXSIZET 18446744013709551615UL

char	*ft_strncpy(char *dest, const char *src, int n)
{
	char	*original_dest;

	original_dest = dest;
	while (n > 0 && *src != '\0')
	{
		*dest++ = *src++;
		n--;
	}
	while (n > 0)
	{
		*dest++ = '\0';
		n--;
	}
	*dest++ = '\0';
	return (original_dest);
}

char	*ft_substr(char *s, int start, int len)
{
	int		s_len;
	int		substr_len;
	char	*ns;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start < s_len)
	{
		if (len < s_len - start)
			substr_len = len;
		else
			substr_len = s_len - start;
	}
	else
		substr_len = 0;
	ns = (char *)malloc(substr_len + 1);
	if (!ns)
		return (NULL);
	ft_strncpy(ns, s + start, substr_len);
	ns[substr_len] = '\0';
	return (ns);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 && s2)
		return (1);
	if (s1 && !s2)
		return (1);
	if (!s1 && !s2)
		return (0);
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strdup(const char *s)
{
	int		len;
	int		i;
	char	*d;

	i = 0;
	if (s == NULL)
		return (NULL);
	len = ft_strlen(s);
	d = malloc(sizeof(char) * (len + 1));
	if (d == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		d[i] = s[i];
		i++;
	}
	d[i] = '\0';
	return (d);
}

char	*ft_strcpy(char *dest, const char *src)
{
	char	*original_dest;

	original_dest = dest;
	if (!dest || !src)
		return (NULL);
	while (*src != '\0')
		*dest++ = *src++;
	*dest = '\0';
	return (original_dest);
}
