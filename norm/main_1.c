/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 18:35:07 by myassine          #+#    #+#             */
/*   Updated: 2024/02/05 23:36:56 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	start_input(t_all *all)
{
	signal(SIGINT, &sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	all->envs = ft_lstsplit(&all->env);
	all->path = get_current_directory_with_prompt();
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
	return (SUCCESS);
}

// all == addresse d une structure t_all
// *all == valeur a l adresse stockee
// &all == addresse de all

// {
// t_lst *lst; lst == addr d un lst  *lst == valeur dans struct lst  &lst == 

// lst = 0xfee7548;

// lst->next = 0xfee7548
// }

// call(&lst);

// {


// lst == 0x1684ade

// *lst = (*lst)->next;

// }

void	end_prompt(t_all *all)
{
	printf("Test\n");
	unlink("heredoc_temp_file.txt");
	dup2(all->saved_stdin, STDIN_FILENO);
	dup2(all->saved_stdout, STDOUT_FILENO);
	close(all->saved_stdin);
	close(all->saved_stdout);
	// signal(SIGINT, SIG_IGN);
	while (1)
	{
		all->pid = waitpid(-1, &all->status, 0);
		if (WIFEXITED(all->status))
			all->status = WEXITSTATUS(all->status);
		if (all->status == 131)
			write(2, "Quit (core dumped)\n", 19);
		if (all->pid < 0)
			break ;
	}
	signal(SIGINT, &sigint_handler);
	if(all->args != NULL)
		free_struct_all(all);
}
void	free_struct_all(t_all *all)
{
	free_string_array(all->args);
	// (void)all->args;
	free_block_list(all->test);
	all->test = NULL;
}

int	check_error_input(t_all *all)
{
	if (!all->input)
	{
		eof(all->input, all->envs, all->env, all->head_env);
		all_free_1(all->test, all->env, all->head_env, all->args);
		exit(0);
	}
	if (all->input[0])
		add_history(all->input);
	if (check_error(all->input))
		print_error(check_error(all->input));
	all->i_a = 0;
	all->command_alone = 0;
	return(check_error(all->input));
}

int		parsing(t_all *all)
{
	while (all->input && all->input[all->i_a])
	{
		if (all->input[all->i_a] == '|')
			all->command_alone++;
		all->i_a++;
	}
	in_quote(all->input);
	all->input = expa_chang(all->input, all->env, all->head_env);
	all->test = NULL;
	if (check_error(all->input))
		return (print_error(check_error(all->input)), 1);
	all->test = tokenization(all->input);
	all->saved_stdout = dup(STDOUT_FILENO);
	all->saved_stdin = dup(STDIN_FILENO);
	return (0);
	// printf(BACK_BOLD_RED"all->test->dir->file: %p"RESET"\n", &all->test->dir->file);
}

void	dup_in_child(t_all *all)
{
	if (all->prev_pipe_fd != -1)
	{
		dup2(all->prev_pipe_fd, STDIN_FILENO);
		close(all->prev_pipe_fd);
	}
	if (all->test->next)
		dup2(all->pipe_fds[1], STDOUT_FILENO);
	close(all->pipe_fds[0]);
	close(all->pipe_fds[1]);
	apply_redirections_to_command_line(all);
	if (all->args && !ft_strcmp(all->test->cmd, "exit" ))
	{
		if (ft_exit_1(all->test))
		{
			eof(all->input, all->envs, all->env, all->head_env);
			exit(ft_atoi(all->test->arg[0]));
		}
	}
}

// .
// $ | $
// $lol | echo mdr.
// "$c" et $c meme comportement 