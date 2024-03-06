/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 18:35:07 by myassine          #+#    #+#             */
/*   Updated: 2024/03/03 20:48:13 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	start_input(t_all *all)
{
	g_ctrl_c = 0;
	if (all->test)
		all->test = NULL;
	if (all->str)
		all->str = NULL;
	signal(SIGINT, &sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	all->path = get_current_directory_with_prompt();
	if (all->envs)
		free_string_array(all->envs);
	all->envs = ft_lstsplit(&all->env);
	all->head_env = all->env;
	if (!all->path)
		return (FAILURE);
	while (all->path[all->i_a])
		all->i_a++;
	if (all->path[--all->i_a] != ' ')
		all->tmp_path = ft_strjoin(all->path, "$> ");
	all->path = all->tmp_path;
	if (!all->path)
		return (0);
	all->input = readline(all->path);
	free(all->tmp_path);
	all->tmp_path = NULL;
	return (SUCCESS);
}

void	end_prompt(t_all *all)
{
	dup2(all->saved_stdin, STDIN_FILENO);
	dup2(all->saved_stdout, STDOUT_FILENO);
	close(all->saved_stdin);
	close(all->saved_stdout);
	while (1)
	{
		all->pid = waitpid(-1, &all->status, 0);
		if (WIFEXITED(all->status))
			all->status = WEXITSTATUS(all->status);
		if (all->status == 131)
		{
			write(2, "Quit (core dumped)\n", 19);
			break ;
		}
		if (all->pid < 0)
			break ;
		if (g_ctrl_c == 130)
		{
			all->status = 130;
			g_ctrl_c = 0;
		}
	}
	signal(SIGINT, &sigint_handler);
}

int	check_error_input(t_all *all)
{
	int	check;

	check = 0;
	if (!all->input)
	{
		free_struct_all(all);
		write(2, "exit\n", 5);
		exit(all->status);
	}
	if (all->input[0])
		add_history(all->input);
	if (check_error(all->input))
		print_error(check_error(all->input));
	all->i_a = 0;
	all->command_alone = 0;
	check = check_error(all->input);
	return (check);
}

int	parsing(t_all *all)
{
	while (all->input && all->input[all->i_a])
	{
		if (all->input[all->i_a] == '|')
			all->command_alone++;
		all->i_a++;
	}
	in_quote(all->input);
	all->input = expa_chang(all->input, all);
	if (!all->input || !*all->input)
		return (1);
	all->test = NULL;
	if (check_error(all->input))
		return (print_error(check_error(all->input)), 1);
	all->test = tokenization(all);
	all->saved_stdout = dup(STDOUT_FILENO);
	all->saved_stdin = dup(STDIN_FILENO);
	return (0);
}

void	dup_in_child(t_all *all)
{
	apply_redirections_to_command_line(all);
	if (all->prev_pipe_fd != -1)
	{
		dup2(all->prev_pipe_fd, STDIN_FILENO);
		close(all->prev_pipe_fd);
	}
	if (all->test->next)
		dup2(all->pipe_fds[1], STDOUT_FILENO);
	close_saved(all);
	close(all->pipe_fds[0]);
	close(all->pipe_fds[1]);
	if (all->args && !ft_strcmp(all->test->cmd, "exit" ))
		all->erreur = 1;
}
