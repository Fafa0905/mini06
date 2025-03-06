/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:06:48 by marvin            #+#    #+#             */
/*   Updated: 2025/02/25 15:21:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lux(t_commandlist *mini)
{
	t_arg *cur;

	cur = mini->cmd->args;
	while (cur)
	{
		if (cur->content[0] == '"' && cur->content[1] == '$')
			cur->content = look_env(mini, cur->content);
		cur = cur->next;
	}
}

char *look_env(t_commandlist *mini, char *tab)
{
	int	i;
	char	*line;

	i = 0;
	tab = ft_strtrim(tab, "\"");
	while (mini->env[i]->line)
	{
		line = ft_strnstr(mini->env[i]->line, (tab+ 1), ft_strlen(tab));
		if (line)
			return (ft_strdup(mini->env[i]->line + ft_strlen(tab) + 1));
		i++;
	}
	return (tab);
}
void	set_env(t_commandlist *mini, char **env)
{
	int	i;

	i = 0;
	if (!env || !*env)
	{
		printf("Y a rien à gratter\n");
		exit(0);
	}
	if (!mini->env) // S'assurer que mini->env est bien alloué
	{
		mini->env = malloc(sizeof(t_lst *));
		if (!mini->env)
			return ;
		*mini->env = NULL;
	}
	while (env[i])
		append_node(mini, env[i++], mini->env);
}

void	append_node(t_commandlist *mini, char *env, t_lst **lst)
{
	t_lst	*newnode;
	t_lst	*last;
	(void)mini;

	if (!lst) // Vérification de lst avant d'accéder à *lst
		return ;

	newnode = malloc(sizeof(t_lst));
	if (newnode == NULL)
		return ;
	newnode->next = NULL;
	newnode->line = ft_strdup(env);
	newnode->pre = NULL;

	if (*lst == NULL) // Si la liste est vide
	{
		*lst = newnode;
	}
	else
	{
		last = find_last(*lst);
		if (last) // Vérifier que find_last a bien trouvé un dernier élément
		{
			last->next = newnode;
			newnode->pre = last;
		}
	}
}

t_lst	*find_last(t_lst *stack)
{
	if (!stack)
		return (NULL);
	while (stack->next)
		stack = stack->next;
	return (stack);
}
