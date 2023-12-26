#include "../minishell.h"

void	redirect_input(char *filename)
{
	int	file;

	file = open(filename, O_RDONLY);
	if (file == -1)
	{
		printf("minishell: %s: No such file or directory\n", filename);
		exit(EXIT_FAILURE);
	}
	dup2(file, STDIN_FILENO);
	close(file);
}

void	apply_redirections_to_command_line(t_block *test, t_env *env,
		t_env *head_env, int saved_stdin)
{
	while (test && test->dir && test->dir->file)
	{
		if (test->dir->type == APPEND)
			append_output(test->dir->file);
		else if (test->dir->type == IN)
			redirect_input(test->dir->file);
		else if (test->dir->type == OUT)
			redirect_output(test->dir->file);
		else if (test->dir->type == HEREDOC)
			redirect_heredoc(test->dir->file, env, head_env, saved_stdin);
		test->dir = test->dir->next;
	}
}

int	is_bultin(char *args)
{
	if (!ft_strcmp(args, "cd"))
		return (1);
	else if (!ft_strcmp(args, "exit"))
		return (1);
	else if (!ft_strcmp(args, "pwd"))
		return (1);
	else if (!ft_strcmp(args, "echo"))
		return (1);
	else if (!ft_strcmp(args, "unset"))
		return (1);
	else if (!ft_strcmp(args, "env"))
		return (1);
	else if (!ft_strcmp(args, "export"))
		return (1);
	return (0);
}

int	is_real_num(const char *num)
{
	int	i;

	i = 0;
	if (num && num[0] != '-' && !is_num(num[0]))
		return (0);
	if (num[0] == '-')
		i++;
	while (num[i])
	{
		if (!is_num(num[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit_1(t_block *block)
{
	dprintf(2, "exit\n");
	if (block->arg && len_tab(block->arg) > 1 && !is_real_num(block->arg[1]))
		return (printf("exit doesn't have the right arguments\n"), 0);
	return (1);
}
