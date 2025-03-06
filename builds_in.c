/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulds_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 12:58:45 by marvin            #+#    #+#             */
/*   Updated: 2025/02/25 12:58:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	build_in(t_commandlist *mini, char *input)
{
	if (ft_strncmp(input, "exite", ft_strlen("exit")) == 0)
		ft_exit(mini, input);
	if (ft_strncmp(input, "echo", ft_strlen("echo")) == 0)
		ft_echo(mini);
	if (ft_strncmp(input, "env", ft_strlen("env")) == 0)
		ft_env(mini);
	if (ft_strncmp(input, "unset", ft_strlen("unset")) == 0)
		ft_unset(mini);
	if (ft_strncmp(input, "export", ft_strlen("export")) == 0)
		ft_export(mini);
	if (ft_strncmp(input, "pwd", ft_strlen("pwd")) == 0)
		ft_pwd(mini);
	if (ft_strncmp(input, "cd", ft_strlen("cd")) == 0)
		ft_cd(mini);
}

void	ft_exit(t_commandlist *mini, char *input)
{
	int	i;
	t_arg *cur;
	t_arg *nxt;

	cur = mini->cmd->args->next;  // Vérifie que mini->cmd->args existe aussi !
	if (!cur)  // Si pas d'arguments après "exit", on sort directement
		clean_up_and_exit(input, mini);

	nxt = cur->next;
	if (!nxt) // Si pas de deuxième argument
	{
		i = ft_atoi(cur->content);
		if (isnum(cur->content) == 0)  // Vérifie si c'est un nombre
			(mini->res = i, clean_up_and_exit(input, mini));
		else
			clean_up_and_exit(input, mini);
	}
	return;
}

void	ft_echo(t_commandlist *mini)
{
	t_arg	*cur;

	cur = mini->cmd->args->next;
	while (cur)
	{
		printf("%s\n",  cur->content);
		cur = cur->next;
	}
}
void	ft_env(t_commandlist *mini)
{
	t_lst	*cur;

	cur = (*mini->env);
	while(cur)
	{
		printf("%s \n", cur->line);
		cur = cur->next;
	}
}
void ft_unset(t_commandlist *mini)
{
    t_lst *cur;
    t_arg *cur_a;
    t_lst *del;
    t_lst *prev;

    cur_a = mini->cmd->args;
    while (cur_a)
    {
		cur = *mini->env;
        prev = NULL;
        while (cur)
        {
            if (ft_strncmp(cur->line, cur_a->content, ft_strlen(cur_a->content)) == 0)
            {
                del = cur;
                cur = cur->next;
                if (del == *mini->env)
                    *mini->env = cur;
                if (prev)
                {
                    prev->next = cur;
                    if (cur)
                        cur->pre = prev;
                }
                deletenode(del);
                break;
            }
        }
        prev = cur;
        cur = cur->next;
    }
	cur_a = cur_a->next;
}


void deletenode(t_lst *out)
{
    if (!out)  // Vérifie si out est NULL
    {
        printf("Erreur : nœud invalide\n");
        return ;
    }
    // Si le nœud a un prédecesseur
    if (out->pre)
    {
        // Si le nœud a un successeur
        if (out->next)
        {
            out->pre->next = out->next;
            out->next->pre = out->pre;
        }
        // Si le nœud est le dernier de la liste
        else
            out->pre->next = NULL;
    }
    // Si le nœud n'a pas de prédecesseur, il est le premier nœud
    else if (out->next)
        out->next->pre = NULL;
    // Finalement, réinitialiser le nœud
    out->pre = NULL;
    out->next = NULL;
}



int	ft_stri(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != c && str[i])
		i++;
	if (str[i])
		return (i);
	return (-1);
}
void	ft_export(t_commandlist *mini)
{
	t_arg *cur;

	if(!mini)
		return ;
	cur = mini->cmd->args;
	while (cur)
	{
		if (ft_stri(cur->content, '=') == -1)
			return ;
			//special_case(mini);
		append_node(mini, ft_substr(cur->content, 0, ft_strlen(cur->content)), mini->env);
		cur = cur->next;
	}
}
void	ft_pwd(t_commandlist *mini)
{
	(void)mini;
	printf("%s\n", getcwd(NULL, 100));
}

void	ft_cd(t_commandlist *mini)
{
	t_arg	*cur;

	cur = mini->cmd->args->next;
	if (cur->next)
		printf("too many arguments\n");
	else if (chdir(cur->content) == -1)
		printf("directory problem\n");
}

int	isnum(char *str)
{
	int	i;

	i = 0;
	while(str[i])
	{
		if(ft_isdigit(str[i]) == 0)
			return(1);
		i++;
	}
	return (0);
}
