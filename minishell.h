/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 00:51:05 by myassine          #+#    #+#             */
/*   Updated: 2023/11/23 21:04:46 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

typedef struct s_dir//a voir
{
	char	***app_redir_doc;
	void	*next;
}	t_dir;

typedef struct s_block
{
	char	*cmd; //commande
	char	**arg; //argument ou option
	void	*next;
	t_dir	*dir;
}	t_block;

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
void	ft_echo(t_env *env, t_env *head_env, char *input);
void	ft_pwd(char *input);
void	ft_cd(char *input);

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
int		nbr_arg(char **token, int j);

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
char	**tab_arg(t_block *block, char **token, int j);

#endif