/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_mini_4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 20:12:03 by myassine          #+#    #+#             */
/*   Updated: 2023/12/29 18:39:14 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#define MAXSIZET 18446744013709551615UL

char	*ft_strstr(const char *big, const char *little)
{
	size_t	a;
	size_t	b;

	a = 0;
	if (little[a] == '\0')
		return ((char *)big);
	while (big[a])
	{
		b = 0;
		while (little[b] == big[a + b])
		{
			if (little[b + 1] == '\0')
				return ((char *)big + a);
			b++;
		}
		a++;
	}
	return (NULL);
}

int	ft_lstsize(t_env *lst)
{
	unsigned int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

int	len_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

void	return_neg(char *str)
{
	int	i;

	if (!str)
		return ;
	i = -1;
	while (str[++i])
		if (str[i] < 0)
			str[i] *= -1;
}

int	nbr_arg(char **token, int j)
{
	int	i;
	int	len;

	i = 0;
	len = j;
	while (token[j])
	{
		if (is_redir(token[j]))
		{
			if (!token[j + 1])
				return (-1);
			j += 2;
			i += 2;
		}
		else
			j++;
	}
	j -= len;
	j -= i;
	return (j);
}
