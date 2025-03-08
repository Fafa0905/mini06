/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 15:43:20 by marvin            #+#    #+#             */
/*   Updated: 2025/02/04 17:58:05 by fsingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal = 0;

t_commandlist	*init_shell(void)
{
	t_commandlist	*mini;

	mini = malloc(sizeof(t_commandlist));
	if (!mini)
		return (NULL);
	mini->tokens = NULL;
	mini->cmd = NULL;
	mini->res = 0;
	return (mini);
}

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = SIGINT;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
		signal(SIGQUIT, SIG_IGN);
}

int	main(int ac, char **argv, char **env)
{
	t_commandlist	*mini;
    char	*input;
	(void)argv;
	(void)ac;
	int	i;

	mini = init_shell();
	if (!mini)
		return (printf("Error allocating memory\n"), 1);
	set_env(mini, env);
	signal(SIGQUIT, signal_handler);
	signal(SIGINT, signal_handler);
	while (1)
	{
		input = readline("user:");
		if (!input)
			break;
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			continue;
		}
		if (*input)
			add_history(input);
		if (parsing(input, mini) != 0)
		{
			i = mini->res;
			free(input);
			free_shell(mini);
			continue;
		}
		free(input);
		free_shell(mini);
	}
	free_shell(mini);
	free(mini);
	rl_clear_history();
	printf("exit\n");
	return (i);
}
