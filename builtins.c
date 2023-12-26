/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 20:54:16 by myassine          #+#    #+#             */
/*   Updated: 2023/12/26 21:35:45 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//si j ai le tps tri l env de export par ordre alpha

int	is_plus(char c)
{
	return (c == '+');
}

int	is_egal(char c)
{
	return (c == '=');
}

void	print_exp(t_env *env, t_env *head_env)
{
	while (env)
	{
		if(env->str[0])
			printf("declare -x %s", env->str[0]);
		if(env->str[1])
			printf("=\"%s\"\n", env->str[1]);
		else if(!env->str[1])
			printf("\n");
		env = env->next;
	}
	env = head_env;
}
// [FONCTION QUI CHECK L'ELEMENT EXPORTABLE, A REVISER SI ERREUR]
int	check_export(char *str)
{
	int i = 0;
	int sig_egal = 0;

	while(str[i] && (is_alpha(str[i]) || is_underscor(str[i])))
		i++;
	if(i == 0)
		return (0);
	while(str[i] && is_alnum(str[i]))
	{
		if(!is_alnum(str[i]))
			return (0);
		i++;
	}
	if(str[i] && is_egal(str[i]))
	{
		i++;
		sig_egal++;
	}
	if(sig_egal)
		return (2);
	return (1);
}

char	*pre_egal(char *str)
{
	char *tmp;
	int i = 0;
	while (str[i] && !is_egal(str[i]))
		i++;
	tmp = malloc(sizeof(char) * (i + 1));
	if(!tmp)
		return (NULL);
	i = 0;
	while (str[i] && !is_egal(str[i])) 
	{
		tmp[i] = str[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

char	*post_egal(char *str)
{
	char *tmp;
	int i = 0;
	int j = 0;
	int k = 0;
	while (str[i] && !is_egal(str[i]))
		i++;
	if(!str[i + 1])
	{
		char *ss = malloc(sizeof(char));
		if(!ss)
			return NULL;
		ss[0] = '\0';
		return (ss);
	}
	i++;
	j = i;
	while(str[i])
		i++;
	k = i - j;
	tmp = malloc(sizeof(char) * (k + 1));
	if(!tmp)
		return NULL;
	i = 0;
	while(str[j])
	{
		tmp[i] = str[j];
		i++;
		j++;
	}
	tmp[i] = '\0';
	return (tmp);
}

int		check_export_exist(t_env *env, t_env *head_env, char *str)
{
	char *pre_e;

	pre_e = pre_egal(str);
	if(!pre_e)
		return (0);
	while(env)
	{
		if(!ft_strcmp(env->str[0], pre_e))
		{
			free(pre_e);
			env = head_env;
			return (1);
		}
		env = env->next;
	}
	env = head_env;
	free(pre_e);
	return (0);
}

t_env	*export_env_1(t_env *env, t_env *head_env, char *str)
{
	t_env *new_block_env;

	new_block_env = malloc(sizeof(t_env));
	if(!new_block_env)
		return NULL;
	new_block_env->str[0] = ft_strdup(str);
	if(!new_block_env->str[0])
		return NULL;
	new_block_env->str[1] = NULL;
	new_block_env->next = NULL;
	while(env->next != NULL)
		env = env->next;
	env->next = new_block_env;
	env = head_env;
	return (env);
}

t_env	*export_env_2(t_env *env, t_env *head_env, char *str)
{
	t_env *new_block_env;

	str = if_quote(str);
	new_block_env = malloc(sizeof(t_env));
	if(!new_block_env)
		return NULL;
	new_block_env->str[0] = pre_egal(str);
	if(!new_block_env->str[0])
		return NULL;
	new_block_env->str[1] = post_egal(str);
	new_block_env->next = NULL;
	if (env)
	{
		while(env->next != NULL)
			env = env->next;
		env->next = new_block_env;
	}
	env = head_env;
	return (env);
}


t_env	*rpl_env_var(t_env *env, t_env *head_env, char *str)
{
	t_env *tmp;

	while(env)
	{
		tmp = env->next;
		char *p_e = pre_egal(str);
		if(!p_e)
			return NULL;//a voir plus tard
		if(env->next && !ft_strcmp(env->next->str[0],p_e))
		{
			free(p_e);
			if (tmp)
			{
				if(tmp->str[1] != NULL)
					free(tmp->str[1]);
				char *te = ft_strdup(str);
				te = if_quote(str);
				tmp->str[1] = post_egal(te);//
				free(te);
				if(!tmp->str[1])
					return (NULL);
			}
			return (env);
		}
		free(p_e);
		env = env->next;
	}
	env = head_env;
	return (env);
}

t_env	*sup_env_var(t_env *env, t_env *head_env, char *str)
{
	t_env *tmp;
	char *str2;
	
	str2 = pre_egal(str);
	while(env && env->next)
	{
		tmp = env->next;
		if (!ft_strcmp(tmp->str[0], str2))
		{
			env->next = tmp->next;
			free(tmp->str[0]);
			free(tmp->str[1]);
			free(tmp);
			free(str2);
			return head_env; // Retourne la tête de la liste mise à jour
		}
		env = env->next;
	}
	free(str2);
	return head_env; // Si la variable n'est pas trouvée, retourne la tête de la liste inchangée
}

void	ft_env(t_env *env, t_env *head_env, t_block *block)
{
	if(!ft_strcmp(block->cmd, "env") && !block->arg)
		print_env(env, head_env);
}

int cut_export_argument(char* arg_export)
{
	char **exported = ft_split_path(arg_export, '=');
	// int i = 0;

	int status_export = check_export(exported[0]);
	if (!status_export)
		return (printf("export: `%s': not a valid identifier\n", exported[0]), SUCCESS);
	
	
	return (SUCCESS);
}

void	ft_export(t_env *env, t_env *head_env, t_block *block)
{
	int		i;
	
	i = 0;
	if(!ft_strcmp(block->cmd, "export") && block->arg == NULL)
	{
			print_exp(env, head_env);
			return;
	}
	if((!ft_strcmp(block->cmd, "export")))
	{
		while(block->arg[i])
		{
			if(!check_export(block->arg[i]))// A VOIR (export a=)-->apparait d en l env ne devrais pas
				printf("export: `%s': not a valid identifier\n", block->arg[i]);
			else if(check_export(block->arg[i]) == 1 
				&& !check_export_exist(env, head_env, block->arg[i]))
				env = export_env_1(env, head_env, block->arg[i]);
			else if(check_export(block->arg[i]) == 1 
				&& check_export_exist(env, head_env, block->arg[i]))
				i = i;
			else if(check_export(block->arg[i]) == 2
				&& !check_export_exist(env, head_env, block->arg[i]))
				env = export_env_2(env, head_env, block->arg[i]);
			else if(check_export(block->arg[i]) == 2
				&& check_export_exist(env, head_env, block->arg[i]))
				env = rpl_env_var(env, head_env, block->arg[i]);
			i++;
		}
	}
}

void	ft_unset(t_env *env, t_env *head_env, t_block *block)
{
	if((!ft_strcmp(block->cmd, "unset")) && (block->arg[0])
		&& (check_export_exist(env, head_env, block->arg[0])))
	{
			sup_env_var(env, head_env, block->arg[0]);
			return;
	}
}

int		is_quote(char *str)
{
	if(str[0] == '\'')
		return (1);
	else if(str[0] == '\"')
		return (2);
	else
		return (0);
}

void printc(char *input)
{
	char c;
	int i = 0;

	while(input[i])
	{
		if(input[i] && (input[i] == '\'' || input[i] == '\"'))
		{
			c = input[i];
			i++;
			while(input[i] && input[i] != c)
			{
				printf("%c", input[i]);
				i++;
			}
			i++;
		}
		else if(input[i] == 32)
		{
			printf(" ");
			skip_whitespace(input, &i);
		}
		else if(input[i])
		{
			printf("%c", input[i]);
			i++;
		}
	}
}

int check_n(char **tab)
{
	int i = 1;
	int j = 1;
	
	while(tab[j] && tab[j][0] == '-' && tab[j][1] && tab[j][1] == 'n')
	{
		while(tab[j][i])
		{
			if(tab[j][i] != 'n')
				return (1);
			i++;
		}
		j++;
	}
	if(j != 1)
		return (2);
	return (0);
}

void	ft_echo(t_block *block)
{
	int x = 0;
	int i;
	int check_n = 0;
	if(!block->arg)
		return ;
	while(block->arg[x])
	{
		i = 1;
		if(block->arg[x][0] != '-')
			break;
		while(block->arg[x][i])
		{
			if(block->arg[x][i] != 'n')
				break;
			check_n++;
			i++;
		}
		if(block->arg[x][i] && block->arg[x][i] != 'n')
			break;
		x++;
	}
	int tab_len = len_tab(block->arg);
	while(block->arg[x])
	{
		printf("%s", block->arg[x]);
		if(x < tab_len - 1)
			printf(" ");
		x++;
	}
	if (!check_n)
		printf("\n");
}

void	ft_putstr_fd(char *s, int fd)
{
	if (*s)
		write(fd, s, ft_strlen(s));
}

void	ft_pwd(t_block *block)
{
	if(!ft_strcmp(block->cmd, "pwd"))
	{
		char *path;
		path = get_current_directory_with_prompt();
		printf("%s\n", path);
	}
}


//echo   eeee"aaa"fff"bbb"gggg       leaks