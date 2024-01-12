/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstsplit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:07:22 by myassine          #+#    #+#             */
/*   Updated: 2024/01/12 19:24:01 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	tmpfunc(char **res, t_env *ptr, int i)
{
	int		h;
	char	*tmp;
	char	*itoa;

	h = ft_atoi(ptr->str[1]);
	tmp = ft_strjoin(ptr->str[0], "=");
	itoa = ft_itoa(++h);
	if (!itoa)
		return (1);
	res[i] = ft_strjoin(tmp, itoa);
	free(itoa);
	if (tmp)
		free(tmp);
	return (!res[i]);
}

int	elsefunc(char **res, t_env *ptr, int i)
{
	char	*tmp;

	tmp = ft_strjoin(ptr->str[0], "=");
	res[i] = ft_strjoin(tmp, ptr->str[1]);
	if (tmp)
		free(tmp);
	return (!res[i]);
}

char	**ft_lstsplit(t_env **lst)
{
	char	**res;
	t_env	*ptr;
	int		size;
	int		i;

	ptr = *lst;
	i = -1;
	size = ft_lstsize(ptr);
	res = (char **)ft_calloc(sizeof(char *), (size + 1));
	if (!res)
		return (NULL);
	while (++i < size)
	{
		if (!ft_strcmp("SHLVL", ptr->str[0]))
		{
			if (tmpfunc(res, ptr, i))
				return (NULL);
			ptr = ptr->next;
			continue ;
		}
		if (elsefunc(res, ptr, i))
			return (NULL);
		ptr = ptr->next;
	}
	return (res);
}
