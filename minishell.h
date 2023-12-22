/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 00:51:05 by myassine          #+#    #+#             */
/*   Updated: 2023/12/22 18:35:52 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#ifndef COLOR_H
# define COLOR_H

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

#endif

#define MAX_ARGS 128
#define PATH_MAX 4096
#define MAX_tkn_SIZE 64
#define MAX_NUM_tknS 64
#define MAX_PATH_LEN 1024
#define MAX_INPUT_SIZE 1024


#define FAILURE 0
#define SUCCESS 1

#define S_QUOTE '\''
#define D_QUOTE '"'
#define R_LEFT '<'
#define R_RIGHT '>'
#define PIPE '|'

////////////////////
//  BIBLIOTHEQUE  //
////////////////////

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <curses.h>
#include <dirent.h>
#include <termios.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <readline/history.h>
#include <readline/readline.h>

////////////////////
//   STRUCTURES   //
////////////////////

//export -> ajoute ou modifie une variable d'env

//unset -> retire une variable d'env
enum e_dir
{
	FILES,
	REDIR,
	NONE
};


typedef	struct s_env
{
	char						*str[2];
	struct  s_env				*next;
}	t_env;

typedef struct s_dir
{
	char 	*file;
	int 	fd;
	int		type;	// void	*next;
	struct s_dir *next;
}				t_dir;

typedef struct s_block
{
	char	*cmd; //commande
	char	**arg; //argument ou option
	int		pipe_out;
	int		pipe_in;
	void	*next;
	t_dir	*dir;
}	t_block;

enum REDIR_TYPES
{
	APPEND,
	IN,
	OUT,
	HEREDOC
};

//Tokenizer
char		**ft_split_path(char const *s, char c);
t_block		*tokenization(char *input);


//Minishell
char	*get_current_directory_with_prompt();
void	eof(char *input, char **envs, t_env *env, t_env *head_env);
void	terminat(char *input,char **envs, t_env *env, t_env *head_env);

//Print
void	print_env(t_env *env, t_env *head_env);
void	print_env1(char *envp[]);
void	print_error(int error);
void	print_input(char *input);
void 	print_tab(char **tab, char *prefix);

//Free
void	free_dir(t_dir *dir);
void	free_block(t_block *block);
void	free_block_list(t_block *head);
void	freeStringArrays(char ***arrays);
void	freeStringArray(char **arrays);
void	terminat(char *input, char **envs, t_env *env, t_env *head_env);

//Expand

void 	expand_variables(char *input);
char 	*ft_get_env(char *exp, t_env *env, t_env *head_env);
char	*expa_chang(char *input, t_env *env, t_env *head_env);
char	*exp_var(char *input, int *i);
int 	len_word_exp(const char *str, int i);

//Bultins
void	ft_export(t_env *env, t_env *head_env, char **envs, char *input);
void	ft_env(t_env *env, t_env *head_env, char *input);
void	ft_unset(t_env *env, t_env *head_env, char *input);
void	ft_echo(t_block *block);
void	ft_pwd(char *input);
void	ft_cd(char *input);

void	ft_exp(t_env *env, t_env *head_env, char **envs, t_block *block);
int is_bultin(char *args);

//Lib_mini
char 	*removeCharAtIndex(char *str, int i);
char	*ft_strchr(char *str, int character);
void    ft_strcpy_len(char **dest, char *src, int start, int end);
int 	ft_strlen_target(char *str, char c);
int		ft_strlen(const char *str);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_strchr_exp(char *str);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strncpy(char *dest, const char *src, int n);
char	*ft_strdup(const char *s);
char    *ft_substr(char *s, int start, int len);
void	return_neg(char *str);
char 	*if_quote(char *input);
char	*ft_itoa(int n);
//static size_t	ft_num_len(long n);
int		ft_atoi(const char *nptr);
char	*ft_strstr(const char *big, const char *little);
int		ft_lstsize(t_env *lst);
int		len_tab(char **tab);
void	return_neg(char *str);
int		nbr_arg(char **token, int j, int *count);

//Check_char

int	    is_re_right(char c);
int	    is_re_left(char c);
int	    is_pipe(char c);
int     is_space(char c);
int	    is_spec_char(char c);

//Check_char_2

int	    is_simple_quote(char c);
int	    is_double_quote(char c);
int		is_doll_sign(char c);
int     is_neg_char(char c);
int		is_num(char c);
int	    is_alnum(int c);
int	    is_alpha(int c);
int		is_underscor(char c);
int		is_redir(char *str);

//Check_quote
int		ft_quote(char *input);
void	in_quote(char *input);
char 	*if_quote(char *input);
char	*no_quote(char *str);

//Check_redir_a_pipe
int		check_redir(char *input);
int		check_pipe(char *input);
int		count_redir(char **token, int j, char *redir);

//Check_error
int		check_error(char *input);
int		str_void(char *input);

//Skip_char

void    skip_whitespace(char *str, int *i);
int 	skip_alnum(char *str, int *i);
int    skip_neg_char(char *str, int *i);
void	skip_word_exp(char *str, int *i);
void	skip_alpha(char *str, int *i);

//Env

void	free_env(t_env *env, t_env *head_env);
int     build_env_2(t_env *env, t_env *head_env, char **envp);
int     build_env(t_env *env, t_env *head_env, char **envp);
int     get_env(t_env *env, t_env *head_env, char **envp);
t_env	*init_env(t_env *env, t_env *head_env, char **envp);

//Malloc
t_dir	*new_dir(char *redir, char *target, t_dir *dir);
t_dir 	*malloc_dir();
t_block *new_block(void);
char 	**add_new_block_tab(char ***tab, char *str);
// char	**tab_arg(t_block *block, char **token, int j);

#endif