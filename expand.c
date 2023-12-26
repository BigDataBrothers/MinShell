/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 02:19:09 by myassine          #+#    #+#             */
/*   Updated: 2023/12/26 20:59:56 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *exp_var(char *input, int *i)
{
    int j = 0;
    int k = 0;
    char *exp = NULL;
    
    while (input[*i])
    {
        if (input[*i + 1] && is_doll_sign(input[*i]) && is_alpha(input[*i + 1]))
        {
            j = *i;
            while ((input[++j]) && (is_alnum(input[j])))
                ;
            if (j == *i + 1)
                return "$";
            k = *i - j;
            exp = ft_substr(&input[*i], *i, k);
            return exp;
        }
        else if (is_doll_sign(input[*i]) && ((!input[*i + 1] || !is_alpha(input[*i + 1]))))
            return "$";
        if (input[*i])
            (*i)++;
    }
    return NULL;
}

int len_word_exp(const char *str, int i)
{
    int length = 1;
    if (str[i + 1])
        i++;
    else
        return 1;
    if (str[i] && (is_num(str[i])))
        return 2;
    else if (str[i] && (is_alpha(str[i]) || is_underscor(str[i])))
    {
        while (str[i] && (is_alnum(str[i]) || is_underscor(str[i])))
        {
            i++;
            length++;
        }
    }
    return length;
}

char *ft_get_env(char *exp, t_env *env, t_env *head_env)
{
    char *tmp = NULL;
    
    while (env)
    {
        if (!ft_strcmp(exp, env->str[0]))
        {
            tmp = ft_strdup(env->str[1]);
            env = head_env;
            return tmp;
        }
        env = env->next;
    }
    env = head_env;
    if(tmp)
        free(tmp);
    return NULL;
}

char *exp_(char *exp, t_env *env, t_env *head_env)
{
    char *tmp = NULL;
    
    if (!exp[1])
        return "$";
    tmp = ft_get_env(exp + 1, env, head_env);
    if (!tmp)
        return NULL;
    return tmp;
}

char *process_exp_variable(char *input, int *i, t_env *env, t_env *head_env)
{
    char *pre_exp = ft_substr(input, 0, *i);
    int len_exp = len_word_exp(input, *i);
    char *exp = ft_substr(input, *i, len_exp);
    int j = *i + ft_strlen(exp);
    char *post_exp = ft_substr(input, j, ft_strlen(input));
    char *tmp = NULL;
    tmp = exp_(exp, env, head_env);
    j = 0;
    if(tmp)
        while(tmp[j])
    {
            if(is_spec_char(tmp[j]))
                tmp[j] *= -1;
            j++;
        }
    char *post_input = ft_strjoin(pre_exp, tmp);
    char *abs = NULL;
    abs = ft_strjoin(post_input, post_exp);
    if(post_input)
        free(post_input);
    post_input = strdup(abs);
    if(input)
        free(input);
    input = strdup(post_input);
    if(post_input)
        free(post_input);
    if(abs)
        free(abs);
    if(post_exp)
        free(post_exp);
    if(exp)
        free(exp);
    if(pre_exp)
        free(pre_exp);
    return input;
}

char *expa_chang(char *input, t_env *env, t_env *head_env)
{
    int i = 0;
    char *tmp = NULL;

    while (input[i] && input[i + 1])
    {
        if (is_doll_sign(input[i]))
        {
            tmp = process_exp_variable(input, &i, env, head_env);
            if (!tmp)
            {
                if(tmp)
                    free(tmp);
                if(input)
                    free(input);
                break;
            }
            input = ft_strdup(tmp);
            if(tmp)
                free(tmp);
        }
        if (is_doll_sign(input[i]) && input[i + 1] && is_alpha(input[i + 1]))
            i--;
        else if (input[i])
            i++;
    }
    return input;
}

