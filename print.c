/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 02:20:08 by myassine          #+#    #+#             */
/*   Updated: 2023/11/16 21:12:29 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(int error)
{
	if(!error)
		return ;
	if(error == 1)
		printf("Syntax error :\nSimple quote is open\n");
	else if(error == 2)
		printf("Syntax error :\nDouble quote is open\n");
	else if(error == 3)
		printf("Syntax error :\nRedirection is fail\n");
	else if(error == 4)
		printf("Syntax error :\nPipe is fail\n");
}//ECRIRE SUR LA SORTI D ERREUR

void	print_env(t_env *env, t_env *head_env)
{
	while (env)
	{
		if(env->str[0] && env->str[1])
			printf("%s=%s\n", env->str[0], env->str[1]);
		env = env->next;
	}
	env = head_env;
}
void	print_env1(char *envp[])
{
	printf("******************************************\n");
	int i = 0;
	while (envp[i]) 
		printf("%s\n", envp[i++]);
	printf("******************************************\n");
}

void	print_input(char *input)
{
	int	i;

	i = -1;
	while(input[++i])
	{
		if(is_neg_char(input[i]))
		{	
			input[i] *= -1;
			printf("%c", input[i]);
		}
		else
			printf("%c", input[i]);
	}
	printf("\n");	
}

void print_tab(char **tab, char *prefix)
{
	int i = 0;
	if(tab[i])
	{
		while (tab && tab[i])
		{
			printf("%s = [%s][%d]\n", prefix, tab[i], i);
			i++;
		}
	}
}