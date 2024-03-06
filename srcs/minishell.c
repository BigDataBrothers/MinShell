/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 00:52:02 by myassine          #+#    #+#             */
/*   Updated: 2024/03/03 21:24:39 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_ctrl_c;

void	for_arg(t_block *test, int *i_a, int *j_a, char **args)
{
	if (test->arg)
	{
		while (test->arg[(*j_a)])
		{
			return_neg(test->arg[(*j_a)]);
			if_quote_2(&test->arg[(*j_a)]);
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

	return_neg(test->cmd);
	if_quote_2(&test->cmd);
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

int	verif_block(t_block *block)
{
	t_block	*tmp;

	tmp = block;
	while (tmp)
	{
		if (!is_space_word(tmp->cmd) && tmp->dir->type == -1)
			return (write(2, "Syntax error :\nPipe is fail\n", 29), 1);
		tmp = tmp->next;
	}
	return (0);
}

void	print_status(t_all *all)
{
	int	i;

	i = -1;
	while (all->test->arg[++i])
	{
		if (!ft_strcmp("$?", all->test->arg[i]))
		{
			free(all->test->arg[i]);
			all->test->arg[i] = NULL;
			all->test->arg[i] = ft_strdup(ft_itoa(all->status));
		}
	}
}

int	main(int argc, char **argv, char *envp[])
{
	static t_all	all = {0};

	init_all(&all, envp);
	(void)argv;
	(void)argc;
	if (argc > 1)
		return ((void)argv, FAILURE);
	while (1)
	{
		if (start_input(&all) == FAILURE)
			return (FAILURE);
		if (check_error_input(&all) || !is_space_word(all.input) \
			|| parsing(&all) || verif_block(all.test))
		{
			freeme(&all, 0);
			continue ;
		}
		if (is_empty_input(&all) == FAILURE)
			exec_all(&all);
		free(all.input);
		free_block_list(all.test);
		unlink("heredoc_temp_file.txt");
	}
	terminat(all.input, all.envs, all.env, all.head_env);
	return (all_free_1(all.test, all.env, all.head_env, all.args), SUCCESS);
}
