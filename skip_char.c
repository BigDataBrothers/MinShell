/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_char.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:38:40 by myassine          #+#    #+#             */
/*   Updated: 2023/10/18 15:31:31 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_whitespace(char *str, int *i)
{
	while (str[*i] && is_space(str[*i]))
		(*i)++;
}

int	skip_alnum(char *str, int *i)
{
	while (str[*i] && !is_spec_char(str[*i]))
		(*i)++;
    return (1);
}

void	skip_alpha(char *str, int *i)
{
	while (str[*i] && is_alpha(str[*i]))
		(*i)++;
}

int    skip_neg_char(char *str, int *i)
{
    while(str[*i] && is_neg_char(str[*i]))
        (*i)++;
    return (1);
}

void	skip_word_exp(char *str, int *i)
{
	if(str[*i + 1])
		(*i)++;
	else
		return;
	if(str[*i] && (is_alpha(str[*i]) || is_underscor(str[*i])))
		while(str[*i] && (is_alnum(str[*i]) || is_underscor(str[*i])))
			(*i)++;
	//printf("char = [%c] char i + 1 = [%c] i = %d\n", str[*i], str[*i + 1], *i);
	if((str[*i] && is_doll_sign(str[*i])) && ((str[*i + 1] && (is_alpha(str[*i + 1]) || is_underscor(str[*i + 1])))))
		skip_word_exp(str, i);	
}
/*
char *ft_strchr_exp(char *str)
{
    char *firstOccurrence;
    int i = 0;

    if (!str)
        return (NULL);
    firstOccurrence = NULL;

    while (str[i])
    {
        if (str[i] == '$')
        {
            firstOccurrence = &str[i];
            i++; // Avance l'itérateur au caractère suivant après '$'
            while (str[i] && (is_alpha(str[i]) || is_underscore(str[i])))
                i++; // Avance l'itérateur jusqu'à la fin du mot
            if (str[i] && is_dollar_sign(str[i]) && (str[i + 1] && (is_alpha(str[i + 1]) || is_underscore(str[i + 1]))))
                i++; // Avance l'itérateur au caractère suivant après le mot
            *firstOccurrence = '\0'; // Remplace le '$' par un caractère nul
            break;
        }
        i++;
    }
    return (firstOccurrence);
}

void copy_word_exp(char *str, char *expand, int *i) {
    int j = 0;

    while (str[*i] && (is_alnum(str[*i]) || is_underscore(str[*i]))) {
        expand[j++] = str[(*i)++];
    }
    expand[j] = '\0';
}

char	*cpy_word_exp(char *str, int *i)
{
	while (str[i]) 
	{
        if (str[i] == '$') 
		{
            i++;
            if (str[i] && (is_alpha(str[i]) || is_underscore(str[i]))) 
			{
                copy_word_exp(str, expand, &i);
                printf("Mot copié : %s\n", expand);
			}

            
        } else 
            i++;
}
	
	if(str[*i + 1])
		(*i)++;
	else
		return;
	if(str[*i] && (is_alpha(str[*i]) || is_underscor(str[*i])))
		while(str[*i] && (is_alnum(str[*i]) || is_underscor(str[*i])))
			(*i)++;
	if((str[*i] && is_doll_sign(str[*i])) && ((str[*i + 1] && (is_alpha(str[*i + 1]) || is_underscor(str[*i + 1])))))
		skip_word_exp(str, i);	
}*/