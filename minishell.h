/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 00:51:05 by myassine          #+#    #+#             */
/*   Updated: 2024/01/25 18:40:52 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>

// color
# define BLACK "\033[0;30m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;35m"
# define CYAN "\033[0;36m"
# define WHITE "\033[0;37m"

// bold color
# define BOLD_BLACK "\033[1;30m"
# define BOLD_RED "\033[1;31m"
# define BOLD_GREEN "\033[1;32m"
# define BOLD_YELLOW "\033[1;33m"
# define BOLD_BLUE "\033[1;34m"
# define BOLD_PURPLE "\033[1;35m"
# define BOLD_CYAN "\033[1;36m"
# define BOLD_WHITE "\033[1;37m"

// background
# define BACK_BLACK "\033[0;40m"
# define BACK_RED "\033[0;41;30m"
# define BACK_GREEN "\033[0;42;30m"
# define BACK_YELLOW "\033[0;43;30m"
# define BACK_BLUE "\033[0;44;30m"
# define BACK_PURPLE "\033[0;45;30m"
# define BACK_CYAN "\033[0;46;30m"
# define BACK_WHITE "\033[0;47;30m"

# define BACK_BOLD_BLACK "\033[1;40m"
# define BACK_BOLD_RED "\033[1;41;30m"
# define BACK_BOLD_GREEN "\033[1;42;30m"
# define BACK_BOLD_YELLOW "\033[1;43;30m"
# define BACK_BOLD_BLUE "\033[1;44;30m"
# define BACK_BOLD_PURPLE "\033[1;45;30m"
# define BACK_BOLD_CYAN "\033[1;46;30m"
# define BACK_BOLD_WHITE "\033[1;47;30m"

// underline color
# define UNDER_BLACK "\033[4;30m"
# define UNDER_RED "\033[4;31m"
# define UNDER_GREEN "\033[4;32m"
# define UNDER_YELLOW "\033[4;33m"
# define UNDER_BLUE "\033[4;34m"
# define UNDER_PURPLE "\033[4;35m"
# define UNDER_CYAN "\033[4;36m"
# define UNDER_WHITE "\033[4;37m"

# define RESET "\033[0m"
# define RST "\033[0m"

# define PATH_MAX 4096

# define FAILURE 0
# define SUCCESS 1

# define S_QUOTE '\''
# define D_QUOTE '"'
# define R_LEFT '<'
# define R_RIGHT '>'
# define PIPE '|'

////////////////////
//  BIBLIOTHEQUE  //
////////////////////

# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <limits.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <curses.h>
# include <dirent.h>
# include <termios.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <readline/history.h>
# include <readline/readline.h>

////////////////////
//   STRUCTURES   //
////////////////////

extern int ret_val;

enum e_dir
{
	FILES,
	REDIR,
	NONE
};

typedef struct s_env
{
	char						*str[2];
	struct s_env				*next;
}	t_env;

typedef struct s_dir
{
	char			*file;
	int				fd;
	int				type;
	struct s_dir	*next;
}				t_dir;

typedef struct s_block
{
	char	*cmd;
	char	**arg;
	int		pipe_out;
	int		pipe_in;
	void	*next;
	t_dir	*dir_head;
	t_dir	*dir;
}	t_block;

typedef struct s_all
{
	char	**envs;
	pid_t	pid;
	t_env	*env;
	t_env	*head_env;
	t_block	*test;
	t_block	*test_head;

	char	*input;
	char	*path;
	char	*str;
	char	*tmp_path;
	char	**args;

	int		pipe_fds[2];
	int		prev_pipe_fd;
	int		saved_stdout;
	int		saved_stdin;
	int		command_alone;
	int		cnt;

	int		i_a;
	int		j_a;
	int		status;
	int		ret_val;
}	t_all;

enum e_REDIR_TYPES
{
	APPEND,
	IN,
	OUT,
	HEREDOC
};

char		**ft_split_path(char const *s, char c);
//Tokenizer
void		tokenization(t_all *all);
t_block		*initialize_and_prepare(char **input);
t_dir		*create_new_dir(void);
void		process_commands(char **split_input, t_block *original);
void		choose_type_redir(char **token, int j, t_dir *new_dir);
void		process_command_token(char **token, int *j, t_block *tmp);
void		process_redirection_token(char **token, int *j, t_block *tmp);
void		allocate_and_assign_arguments(char **token, int *j, t_block *tmp);
void		free_start_dir(t_dir *dir);
void		add_to_special(char *result, char *str, int *i, int *j);
void		cleanup_memory(char **split_input);
int			treat_token(char **token, int *j, t_block *tmp);
int			treat_cmd_line(char *cmd_line, t_block *tmp);
char		*add_spaces(char *str);

//Minishell
char		*get_current_directory_with_prompt(void);
void		eof(char *input, char **envs, t_env *env, t_env *head_env);
void		terminat(char *input, char **envs, t_env *env, t_env *head_env);
char		*remove_char_at_index(char *str, int i);

//Print
void		print_env(t_env *env, t_env *head_env);
void		print_env1(char *envp[]);
void		print_error(int error);
void		print_input(char *input);
void		print_tab(char **tab, char *prefix);

//Free
void		if_free(char *ptr);
void		free_block(t_block *block);
void		free_block_list(t_block *head);
void		freeStringArrays(char ***arrays);
void		free_string_array(char **arrays);
void		terminat(char *input, char **envs, t_env *env, t_env *head_env);
void		free_struct_all(t_all *all);
void		free_struct_dir(t_dir *dir);

//Expand
void		expand_variables(char *input);
void		expa_chang(t_all *all);
char		*exp_(char *exp, t_env *env, t_env *head_env);
void		if_free(char *ptr);
void		free_exp(char **v);

//Expand_2
int			len_word_exp(const char *str, int i);
char		*ft_get_env(char *exp, t_env *env, t_env *head_env);
char		*exp_var(char *input, int *i);

//Bultins
int			is_plus(char c);
int			is_egal(char c);
void		print_exp(t_env *env, t_env *head_env);
int			check_export(char *str);
char		*pre_egal(char *str);

void		malloc_z(char *tmp);
char		*post_egal(char *str);
int			check_export_exist(t_env *env, t_env *head_env, char *str);
t_env		*export_env_1(t_all *all, char *str, int do_free);
t_env		*export_env_2(t_env *env, t_env *head_env, char *str);

int			rpl_en_var_2(char *str, t_env *tmp);
t_env		*rpl_env_var(t_env *env, t_env *head_env, char *str);
t_env		*sup_env_var(t_env *env, t_env *head_env, char *str);
void		ft_env(t_env *env, t_env *head_env, t_block *block);
int			cut_export_argument(char *arg_export);

void		ft_export(t_env *env, t_env *head_env, t_block *block, t_all *all);
void		ft_unset(t_env *env, t_env *head_env, t_block *block);
int			is_quote(char *str);
void		printc(char *input);
int			check_n(char **tab);

int			ft_echo_1(t_block *block, int *i, int *x, int *check_n);
void		ft_echo(t_block *block);
void		ft_putstr_fd(char *s, int fd);
void		ft_pwd(t_block *block);

//Lib_mini
void		ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t count, size_t size);
char		*ft_strjoin(char *s1, char *s2);
char		*ft_strchr(char *str, int character);
int			ft_strlen(const char *str);

//Lib_mini_2
char		*ft_strncpy(char *dest, const char *src, int n);
char		*ft_substr(char *s, int start, int len);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strdup(const char *s);
char		*ft_strcpy(char *dest, const char *src);

//Lib_mini_3
void		ft_strcpy_len(char **dest, char *src, int start, int end);
int			ft_strlen_target(char *str, char c);
int			ft_atoi(const char *nptr);
size_t		ft_num_len(long n);
char		*ft_itoa(int n);

//Lib_mini_4
char		*ft_strstr(const char *big, const char *little);
int			ft_lstsize(t_env *lst);
int			len_tab(char **tab);
void		return_neg(char *str);
int			nbr_arg(char **token, int j);

//Check_char
int			is_re_right(char c);
int			is_re_left(char c);
int			is_pipe(char c);
int			is_space(char c);
int			is_spec_char(char c);

//Check_char_2
int			is_simple_quote(char c);
int			is_double_quote(char c);
int			is_doll_sign(char c);
int			is_neg_char(char c);
int			is_num(char c);
int			is_alnum(int c);
int			is_alpha(int c);
int			is_underscor(char c);
int			is_redir(char *str);

//Check_quote
int			process_quotes(char *input, int i, int *s, int *d);
int			handle_simple_quote(int s);
int			handle_double_quote(int d);
int			process_dollar_sign(char *input, int i);
int			check_quotes(int s, int d);

//Chevk_quote_2
int			ft_quote(char *input);
void		in_quote(char *input);
char		*if_quote(char *input);
char		*no_quote(char *str);

//Check_redir_a_pipe
int			check_redir(char *input);
int			check_pipe(char *input);
int			count_redir(char **token, int j, char *redir);

//Check_error
int			check_error(char *input);
int			str_void(char *input);

//Skip_char
void		skip_whitespace(char *str, int *i);
int			skip_alnum(char *str, int *i);
int			skip_neg_char(char *str, int *i);
void		skip_word_exp(char *str, int *i);
void		skip_alpha(char *str, int *i);

//Env
void		free_env(t_env *env, t_env *head_env);
int			build_env_2(t_env *env, t_env *head_env, char **envp);
int			build_env(t_env *env, t_env *head_env, char **envp);
int			get_env(t_env *env, t_env *head_env, char **envp);
t_env		*init_env(t_env *env, t_env *head_env, char **envp);

// norm
char		*ft_strdupf(char *s);
int			start_input(t_all *all);
void		init_all(t_all *all, char **envp);
void		end_prompt(t_all *all);
int			check_error_input(t_all *all);
int			parsing(t_all *all);
void		dup_in_child(t_all *all);
void		prepare_block(t_all *all);
void		exec_command_alone(t_all *all);
void		exec_all(t_all *all);
char		*ft_strjoin_lfree(char *s1, char *s2);
char		*ft_strjoin_rfree(char *s1, char *s2);
char		*ft_strjoin_dfree(char *s1, char *s2);

//Malloc
t_dir		*new_dir(char *redir, char *target, t_dir *dir);
t_dir		*malloc_dir(void);
t_block		*new_block(void);
char		**add_new_block_tab(char ***tab, char *str);

//mini1.c
char		**get_path(char **env);
int			no_input(char *input);

//mini2.c
void		sigint_handler(int sig);
void		all_free_1(t_block *test, t_env *env, t_env *head_env, char **args);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		redirect_output(char *filename, t_all *all);
void		append_output(char *filename, t_all *all);

//mini3.c
void		redirect_input(char *filename, t_all *all);
void		apply_redirections_to_command_line(t_all *all);
int			is_bultin(char *args);
int			is_real_num(const char *num);
int			ft_exit_1(t_block *block);

//mini4.c
int	applic_bulltin(t_all *all);

//minishell.c
void		redirect_heredoc(char *delimiter, int saved_stdin, t_all *all);

//lstsplit.c
char		**ft_lstsplit(t_env **lst);
char		*verif_cmd(char **args, char **env);
char		**creat_args(t_block *test, int *i_a, int *j_a);
int			is_empty_input(t_all *all);

#endif
