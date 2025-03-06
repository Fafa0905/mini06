/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:52:32 by fsingh            #+#    #+#             */
/*   Updated: 2025/03/06 22:49:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command *init_command(void)
{
	t_command *new_command;
	
	new_command = malloc(sizeof(t_command));
	//printf("Nouvelle commande créée -> Adresse : %p\n", (void *)new_command);
	if (!new_command)
		return (NULL);
	new_command->args = NULL;
	new_command->infile = NULL;
	new_command->outfile = NULL;
	new_command->append = NULL;
	new_command->heredoc = NULL;
	new_command->next = NULL;
	new_command->prev = NULL;
	new_command->pipe[0] = -1;
	new_command->pipe[1] = -1;
	//if (pipe(new_command->pipe) < 0)
	//	return(printf("looose\n"), NULL) ;
	return (new_command);
}

void	add_arg(t_arg **arg_list, char *str)
{
	t_arg *temp;
	t_arg *new_arg;
	
	new_arg = malloc(sizeof(t_arg));
	if (!new_arg)
		return;
	new_arg->content = strdup(str);
	//printf("Ajout d'un argument/redirection -> Adresse : %p | Contenu : %p (%s)\n", (void *)new_arg, (void *)new_arg->content, new_arg->content);
	if (!new_arg->content)
	{
		free(new_arg);
		return;
	}
	new_arg->next = NULL;
	if (!*arg_list)
		*arg_list = new_arg;
	else
	{
		temp = *arg_list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_arg;
	}
}

void	add_token_cmd(t_command *current, t_token_type token_type, char *str)
{
	if (token_type == TOKEN_ARG)
		add_arg(&current->args, str);
	else if (token_type == TOKEN_IN)
		add_arg(&current->infile, str);
	else if (token_type == TOKEN_OUT)
		add_arg(&current->outfile, str);
	else if (token_type == TOKEN_APPEND)
		add_arg(&current->append, str);
	else if (token_type == TOKEN_HEREDOC)
		add_arg(&current->heredoc, str);
}
int	parse_token(t_commandlist *mini)
{
	t_token		*current_token;
	t_command	*current;
	t_command	*temp;
	t_command	*last; // Pour garder une référence au dernier élément ajouté

	current_token = mini->tokens;
	current = NULL;
	last = NULL; // Initialisation du dernier élément
	(void)temp;

	while (current_token)
	{
		if (!current)
			current = init_command();

		if (current_token->type == TOKEN_PIPE)
		{
			if (current && current->args)
			{
				if (!mini->cmd)
				{
					mini->cmd = current;
				}
				else
				{
					last->next = current;
					current->prev = last; // Ajout du chaînage arrière
				}
				last = current; // Mise à jour du dernier élément
			}
			current = NULL;
		}
		else
		{
			add_token_cmd(current, current_token->type, current_token->value);
		}

		current_token = current_token->next;
	}

	// Ajouter la dernière commande si elle existe
	if (current && current->args)
	{
		if (!mini->cmd)
			mini->cmd = current;
		else
		{
			last->next = current;
			current->prev = last; // Ajout du chaînage arrière
		}
	}

	return (0);
}
