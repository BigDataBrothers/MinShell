/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 00:52:02 by myassine          #+#    #+#             */
/*   Updated: 2024/02/08 01:13:59 by myassine         ###   ########.fr       */
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

void	close_saved(t_all *all)
{
	close(all->saved_stdin);
	close(all->saved_stdout);
}

int is_space_word(char *str)
{
	int i;
	
	i = -1;
	if(!str)
		return (0);
	while(str[++i])
		if(!is_space(str[i]))
			return (1);
	return (0);
}

int	verif_block(t_block* block)
{
	t_block	*tmp;

	tmp = block;
	while(tmp)
	{
		if(!is_space_word(tmp->cmd) && tmp->dir->type == -1)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	main(int argc, char **argv, char *envp[])
{
	static t_all	all = {0};

	init_all(&all, envp);
	if (argc > 1)
		return (FAILURE);
	while (1)
	{
		if (start_input(&all) == FAILURE)
			return (FAILURE);
		if (!all.input)
			break ;
		if (!*all.input || !is_space_word(all.input))
			continue;
		if(check_error_input(&all))
			continue ;
		if(parsing(&all))
			continue ;
		if(verif_block(all.test))
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 51);
			close_saved(&all);
			continue ;
		}
		if (is_empty_input(&all) == FAILURE)
			exec_all(&all);
		close_saved(&all);
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
