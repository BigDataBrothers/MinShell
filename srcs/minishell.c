/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 00:52:02 by myassine          #+#    #+#             */
/*   Updated: 2024/02/23 19:00:26 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
int	g_ctrl_c;

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

int	is_space_word(char *str)
{
	int	i;

	i = -1;
	if (!str)
		return (0);
	while (str[++i])
		if (!is_space(str[i]))
			return (1);
	return (0);
}

int	verif_block(t_block *block)
{
	t_block	*tmp;

	tmp = block;
	while (tmp)
	{
		if (!is_space_word(tmp->cmd) && tmp->dir->type == -1)
			return (1);
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

void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_ctrl_c = 130;
		write(2, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(int argc, char **argv, char *envp[])
{
	static t_all	all = {0};

	g_ctrl_c = 0;
	init_all(&all, envp);
	if (argc > 1)
		return (FAILURE);
	while (1)
	{
		g_ctrl_c = 0;
		if (start_input(&all) == FAILURE)
			return (FAILURE);
		printf(YELLOW"all->input: %p"RESET"\n", all.input);
		if (check_error_input(&all))
			continue ;
		if (!is_space_word(all.input))
			continue ;
		if (parsing(&all))
			continue ;
		if (verif_block(all.test))
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 51);
			close_saved(&all);
			continue ;
		}
		if (is_empty_input(&all) == FAILURE)
			exec_all(&all);
		free(all.input);
		free_block_list(all.test);
		close_saved(&all);
		unlink("heredoc_temp_file.txt");
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
		if (!is_space(all->input[i]) && all->input[i] != 9)
			return (FAILURE);
	}
	return (SUCCESS);
}
