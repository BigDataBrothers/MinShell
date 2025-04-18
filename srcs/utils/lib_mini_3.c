/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_mini_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 20:11:57 by myassine          #+#    #+#             */
/*   Updated: 2023/12/28 20:36:26 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#define MAXSIZET 18446744013709551615UL

void	ft_strcpy_len(char **dest, char *src, int start, int end)
{
	int	i;

	i = 0;
	while (src[start])
	{
		if (start == end)
			break ;
		(*dest)[i] = src[start];
		start++;
		i++;
	}
	(*dest)[i] = '\0';
}

int	ft_strlen_target(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (i);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	nb;
	int	minus;

	i = 0;
	nb = 0;
	minus = 1;
	while (nptr[i] && (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13)))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			minus *= -1;
		i++;
	}
	while ((nptr[i]) && (nptr[i] >= '0' && nptr[i] <= '9'))
	{
		if ((nptr[i] >= '0' && nptr[i] <= '9'))
			nb = nb * 10 + (nptr[i] - '0');
		i++;
	}
	return (nb * minus);
}

size_t	ft_num_len(long n)
{
	int	i;

	i = 0;
	if (n <= 0)
		i++;
	while (n != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	long	nb;
	int		dlen;
	char	*dst;

	nb = (long) n;
	dlen = ft_num_len(nb);
	dst = malloc(sizeof(char) * dlen + 1);
	if (!dst)
		return (NULL);
	dst[dlen] = '\0';
	while (--dlen >= 0 && nb != 0)
	{
		if (nb < 0)
			nb = -nb;
		*(dst + dlen) = nb % 10 + '0';
		nb = nb / 10;
	}
	if (n < 0)
		dst[dlen] = '-';
	if (n == 0)
		dst[dlen] = '0';
	return (dst);
}
