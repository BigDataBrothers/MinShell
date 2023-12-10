/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_mini.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 02:17:16 by myassine          #+#    #+#             */
/*   Updated: 2023/12/10 17:46:58 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strjoin(const char *s1, const char *s2)
{
    if (!s1 && !s2)
        return NULL;
    if (!s1)
        return strdup(s2);
    if (!s2)
        return strdup(s1);

    int len1 = ft_strlen(s1);
    int len2 = ft_strlen(s2);
    char *s3 = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
    if (!s3)
        return NULL;

    ft_strcpy(s3, s1);
    ft_strcpy(s3 + len1, s2);

    return s3;
}

char *ft_strchr(char *str, int character)
{
	int i = 0;
	char *firstOccurrence;

	if (!str)
		return (NULL);
	firstOccurrence = NULL;

	while (*str != '\0')
	{
		if (*str == character)
		{
	
			firstOccurrence = str;
			while(firstOccurrence[i] && (!is_neg_char(str[i]))){
				i++;
			}
			firstOccurrence[i] = '\0';
			break;
		}
		str++;
	}
	return (firstOccurrence);
}

int ft_strlen(const char *str)
{
    int i = 0;
    if (!str)
        return 0;
    while (str[i])
        i++;
    return i;
}

char	*ft_strncpy(char *dest, const char *src, int n)
{
    char *original_dest = dest;

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
    return original_dest;
}

char    *ft_substr(char *s, int start, int len)
{
    if (!s)
        return (NULL);

    int 	s_len;
    int	substr_len;
	char	*ns;
	
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

int ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if(!s1)
		return (0);
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (1);
		i++;
	}
	return (0);
}

char *ft_strdup(const char *s)
{
    if (s == NULL)
        return NULL;

    int len;
    char *d;
    
    len = ft_strlen(s);
    d = malloc(sizeof(char) * (len + 1));
    if (d == NULL)
        return NULL;
    int i = 0;
    while (s[i] != '\0')
    {
        d[i] = s[i];
        i++;
    }
    d[i] = '\0';
    return d;
}


char *ft_strcpy(char *dest, const char *src)
{
    if (!dest || !src)
        return NULL;
    char *original_dest = dest;
    while (*src != '\0')
        *dest++ = *src++;
    *dest = '\0';
    return original_dest;
}


void    ft_strcpy_len(char **dest, char *src, int start, int end)
{
    int i;
    
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

int 	ft_strlen_target(char *str, char c)
{   
    int  i;

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

static size_t	ft_num_len(long n)
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
	while(tab[i])
		i++;
	return (i);
}

void	return_neg(char *str)
{
	if(!str)
		return ;
	int i = -1;
	while(str[++i])
		if(str[i] < 0)
			str[i] *= -1;
}

int		nbr_arg(char **token, int j, __attribute__((unused))int *count)
{
	int i = 0;
	int len = j;
	
	while(token[j])
	{
	//	printf("token[%d] : %s\n", j, token[j]);
		if(is_redir(token[j]))
		{
			if (!token[j + 1])
				return (-1);
	//		printf("redir oh : %s\n", token[j]);
			j+=2;
			i+=2;
			
			// return (j);
		}
		else
			j++;
	}
	j -= len;
	j -= i;
	// j -= *count;
	//printf("i = %d || j = %d\n", i , j);
	return (j);
}