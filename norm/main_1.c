/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 18:35:07 by myassine          #+#    #+#             */
/*   Updated: 2024/01/28 21:40:25 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	start_input(t_all *all)
{
	//  signal(SIGQUIT, sigquit_handlers);
	signal(SIGINT, &sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	if (all->envs)
	{
		free_string_array(all->envs);
	}
	// printf(BLUE"res1%p"RST, all->envs);
	all->envs = ft_lstsplit(&all->env);
	// printf(BLUE"res2%p"RST, all->envs);
	all->path = get_current_directory_with_prompt();
	if (!all->path)
		return (FAILURE);
	while (all->path[all->i_a])
		all->i_a++;
	if(all->tmp_path && all->cnt != 0)
	{
		free(all->tmp_path);
		all->tmp_path = NULL;
	}
	if (all->path[--all->i_a] != ' ')
		all->tmp_path = ft_strjoin(all->path, "$> ");
	/*all->path = */ft_strcpy(all->path, all->tmp_path);
	if (!all->path)
		return (0);
	if (all->input)
	{
		free(all->input);
		all->input = NULL;
	}
	all->input = readline(all->path);
	return (SUCCESS);
}

void	end_prompt(t_all *all)
{
	int statu;
	
	statu = 0;
	unlink("heredoc_temp_file.txt");
	dup2(all->saved_stdin, STDIN_FILENO);
	dup2(all->saved_stdout, STDOUT_FILENO);
	close(all->saved_stdin);
	close(all->saved_stdout);
	signal(SIGINT, SIG_IGN);
	while (1)
	{
		all->pid = waitpid(-1, &statu, 0);
		if (all->pid < 0)
			break ;
	}
	//free_env(all->env, all->head_env);
	if (all->status == -1)
	{
		all->status = statu;
		all->status = all->status / 256;
	}
	if (all->args != NULL)
		free_struct_all(all);
}

void	free_struct_all(t_all *all)
{
	// free_env(all->env, all->head_env);
	free_string_array(all->envs);
	all->envs = NULL;
	if_free(all->tmp_path);
	all->tmp_path = NULL;
	// (void)all->args;
	// while(all->args[i])
		// printf("all->args = %p\n", all->args[0]);
	free_string_array(all->args);
	// free(all->args[0]);
	all->args = NULL;
	// free_string_array(all->test);
	// printf("all->str = %p\n", all->str);
	// if_free(all->str);
	// all->str = NULL;
	// free_struct_dir(all->test_head->dir_head);
	free_block_list(all->test_head);
	all->test = NULL;
	all->test_head = NULL;
	if_free(all->input);
	all->input = NULL;
}

int is_space_cp(char *str)
{
	int	i;

	i = -1;
	while(str[++i])
		if(!is_space(str[i]))
			return (0);
	return (1);
}

int	check_error_input(t_all *all)
{
	if (!all->input)
	{
		if (all->envs)
			free_string_array(all->envs);
		printf("exit\n");
		eof(all->input, all->envs, all->env, all->head_env);
		free(all->tmp_path);
		all->tmp_path = NULL;
		all_free_1(all->test, all->env, all->head_env, all->args);
		exit(0);
	}
	if (all->input[0])
		add_history(all->input);
	if (check_error(all->input))
		print_error(check_error(all->input));
	all->i_a = 0;
	all->command_alone = 0;
	return (check_error(all->input));
}

int	parsing(t_all *all)
{
	char *tmp;
	
	while (all->input && all->input[all->i_a])
	{
		if (all->input[all->i_a] == '|')
			all->command_alone++;
		all->i_a++;
	}
	in_quote(all->input);
	// printf(PURPLE"all->input: %p"RESET"\n", all->input);
	tmp = all->input;
	all->input = expa_chang(all->input, all);
	if (tmp != all->input)
		free(tmp);
	// printf(PURPLE"all->input: %p"RESET"\n", all->input);
	all->test = NULL;
	if (check_error(all->input))
		return (print_error(check_error(all->input)), check_error(all->input));
	// free_block_list(all->test);
	tokenization(all);
	all->saved_stdout = dup(STDOUT_FILENO);
	all->saved_stdin = dup(STDIN_FILENO);
	return (0);
}

void	dup_in_child(t_all *all)
{
	if (all->prev_pipe_fd != -1)
	{
		if (dup2(all->prev_pipe_fd, STDIN_FILENO) == -1)
			exit(EXIT_FAILURE);
		close(all->prev_pipe_fd);
	}
	if (all->test->next)
	{
		if (dup2(all->pipe_fds[1], STDOUT_FILENO) == -1)
			exit(EXIT_FAILURE);
		close(all->pipe_fds[1]);
	}
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
