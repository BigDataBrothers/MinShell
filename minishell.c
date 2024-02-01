/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 00:52:02 by myassine          #+#    #+#             */
/*   Updated: 2024/02/01 22:59:52 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_char_at_index(char *str, int i)
{
	char	*new_str;
	int		j;
	int		k;

	j = 0;
	k = 0;
	new_str = (char *)ft_calloc(ft_strlen(str), 1);
	if (str == NULL || i < 0)
		return (NULL);
	if (new_str == NULL)
		return (NULL);
	while (str[k] != '\0')
	{
		if (k != i)
			new_str[j++] = str[k];
		k++;
	}
	return (new_str);
}

void	for_arg(t_block *test, int *i_a, int *j_a, char **args)
{
	// char	*tmp;
	
	if (test->arg)
	{
		while (test->arg[(*j_a)])
		{
			return_neg(test->arg[(*j_a)]);
			// tmp = test->arg[(*j_a)];
			/*test->arg[(*j_a)] = */if_quote_2(&test->arg[(*j_a)]);
			// free(tmp);
			// tmp = NULL;
			args[(*i_a)] = ft_strdup(test->arg[(*j_a)]);
			(*i_a)++;
			(*j_a)++;
		}
		args[(*i_a)] = NULL;
	}
	(*i_a) = 0;
}

char	**creat_args(t_block *test, int *i_a, int *j_a)
{
	char	**args;
	// char	*tmp;

	return_neg(test->cmd);
	// tmp = test->cmd;
	/*test->cmd = */if_quote_2(&test->cmd);
	// if (!test->arg || !test->arg[0])
	// {
		// free(tmp);
		// tmp = NULL;
	// }
	if (!test->arg)
		args = malloc(sizeof(char *) * 2);
	else
		args = malloc(sizeof(char *) * (len_tab(test->arg) + 2));
	if (!args)
		return (NULL);
	(*i_a) = 1;
	(*j_a) = 0;
	args[0] = ft_strdup(test->cmd);
	args[(*i_a)] = NULL;
	for_arg(test, &(*i_a), &(*j_a), args);
	while (args[(*i_a)++])
		return_neg(args[(*i_a)]);
	return (args);
}

void	free_tab(char **args)
{
	int	i_a;

	i_a = -1;
	while (args[++i_a])
	{
		free(args[i_a]);
		args[i_a] = NULL;
	}
}

int	main(int argc, char **argv, char *envp[])
{
	t_all	all;

	init_all(&all, envp);
	if (argc > 1)
		return (FAILURE);
	while (1)
	{
		if (start_input(&all) == FAILURE)
			return (FAILURE);
		check_error_input(&all);
		parsing(&all);
		if (is_empty_input(&all) == FAILURE)
			exec_all(&all);
	}
	terminat(all.input, all.envs, all.env, all.head_env);
	argv = argv;
	return (all_free_1(all.test, all.env, all.head_env, all.args), SUCCESS);
}

int	is_empty_input(t_all *all)
{
	int	i;

	i = -1;
	while (all->input && all->input[++i])
	{
		if (all->input[i] != ' ' && all->input[i] != 9)
			return (FAILURE);
	}
	return (SUCCESS);
}
