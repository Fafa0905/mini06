/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 16:44:19 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/02/04 19:02:05 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_H
# define MINI_H

# include "./libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct s_lst
{
    char *line;
    struct s_lst *next;
    struct s_lst *pre;
}               t_lst;

typedef enum e_token_type
{
    TOKEN_ARG,
    TOKEN_PIPE,
    TOKEN_OUT,
    TOKEN_IN,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
} t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
} t_token;

typedef struct s_arg
{
    char        *content;
	t_lst		*doc;
    struct s_arg *next;
} t_arg;

typedef struct s_command
{
    t_arg      *args;
    t_arg      *infile;
    t_arg       *outfile;
    t_arg      *append;
    int         pipe[2];
    t_arg      *heredoc;
    struct s_command *prev;
    struct s_command *next;
} t_command;

typedef struct s_commandlist
{
    t_token                 *tokens;
    t_lst					**env;
    t_command               *cmd;
    int                     res;
}                            t_commandlist;

//Parsing
void    handle_single_redirect(char *input, int *i, t_commandlist *mini, t_token_type token_type);
void    handle_pipe(char *input, int *i, t_commandlist *mini, t_token_type token_type);
int 	handle_arguments(char *input, int *i, t_commandlist *mini);
int 	lexing(char *input, t_commandlist *mini);
int	    parsing(char *input, t_commandlist *mini);

//Parsing2
t_command   *init_command(void);
int	        parse_token(t_commandlist *mini);

//Utils_Parsing_1
int     ispace(char c);
int     is_special(char c);
int	    only_space(char *input);
int	    open_quote(char *input);
char    *ft_strndup(const char *s, size_t n);

//utils_parsing_2
int	    checking_error_before(char *input);
int	    add_token(t_commandlist *mini, t_token_type type, char *token_value);
void    handle_double_redirect(char *input, int *i, t_commandlist *mini, t_token_type token_type);

//free
void    free_shell(t_commandlist    *mini);
void    clean_up_and_exit(char *input, t_commandlist *mini);
void    free_split(char **split);

//env
void	lux(t_commandlist *mini);
char    *look_env(t_commandlist *mini, char *tab);
void	set_env(t_commandlist *mini, char **env);
void	append_node(t_commandlist *mini, char *env, t_lst **lst);
t_lst	*find_last(t_lst *stack);

//build_in
void	build_in(t_commandlist *mini, char *input);
void	ft_exit(t_commandlist *mini, char *input);
void	ft_echo(t_commandlist *mini);
void	ft_env(t_commandlist *mini);
void ft_unset(t_commandlist *mini);
void	ft_export(t_commandlist *mini);
void	ft_pwd(t_commandlist *mini);
void	ft_cd(t_commandlist *mini);


// utils_build
void deletenode(t_lst *out);
int	ft_stri(char *str, char c);
int	isnum(char *str);

void	print_args(t_commandlist *mini);
void print_commands(t_command *cmd);


//exe
void exe(t_commandlist *mini);
char **findpath(t_commandlist *mini);
void child_pr(t_command *cmd, t_commandlist *mini);
char **lst_to_tab(t_lst **lst);
void cas(t_commandlist *mini);
int list_len(t_lst **lst);
int lefork(void);
void red(t_command *cur);
void in_file(t_command *cur);
void out_file(t_command *cur);
void add_file(t_command *cur);
void her_doc(t_command *cur);
void redirect_lines(t_lst **lst);
void	free_tab(char **tab);
int	ft_strnstri(const char *haystack, const char *needle, size_t len);
void clean_lst(t_lst **lst);
char	**arg_to_tab(t_arg *lst);
int	cmd_len(t_command *cmd);
int	arg_len(t_arg **ar);
# endif