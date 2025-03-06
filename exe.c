/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 12:55:00 by marvin            #+#    #+#             */
/*   Updated: 2025/02/26 12:55:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    exe(t_commandlist *mini)
{
    t_command *cur;
    int i;
	int	*id;
	int	j;
	int	status;

	status = 0;
    cur = mini->cmd;
    i = cmd_len(mini->cmd);
	id = malloc(sizeof(int) * i);
	j = 0;
    while(i--)
    {
		printf("cur: %p, prev: %p, next: %p, pipe[0]: %d, pipe[1]: %d\n",
			(void *)cur, (void *)cur->prev, (void *)cur->next, cur->pipe[0], cur->pipe[1]);	 
		red(cur);
		id[j] = lefork();
		if (id[j] == 0)
		{  
			printf("Processus enfant lancé !\n");
			child_pr(cur, mini);
			ft_putstr_fd("command error : ", 2);
			ft_putstr_fd(mini->cmd->args->content, 2);
			ft_putstr_fd("\n", 2);
			ft_exit(mini, NULL);
		}
		j++;
		cur = cur->next;
    }
	for (int k = 0; k < i; k++)
   		waitpid(id[k], &status, 0);


}

int	cmd_len(t_command *cmd)
{
	int	i;
	t_command *cur;

	cur = cmd;
	i = 0;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	return (i);
}

char **findpath(t_commandlist *mini)
{
	char	*line;
	char	**paths;
	t_lst	*cur;

	cur = *mini->env;
	while (cur)
	{
		if (ft_strnstri(cur->line, "PATH", 4) == 1)
		{
			line = ft_substr(cur->line, 5, ft_strlen(cur->line));
			paths = ft_split(line, ':');
			free(line);
			return (paths);
		}
		cur = cur->next;
	}
	return (NULL);
}

void	child_pr(t_command *cmd, t_commandlist *mini)
{
	char **paths;
	char	*cmde;
	char	**argss;
	char	*tmp;
	int		i;
	char	**env;

	env = lst_to_tab(mini->env);
	(void)env;
	paths = findpath(mini);
	i = 0;
	cas(mini);
	argss = arg_to_tab(cmd->args);
	while (paths && paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		cmde = ft_strjoin(tmp, argss[0]);
		printf("Tentative d'exécution : %s\n", argss[0]);
		printf("Chemin complet testé : %s\n", cmde);
		free(tmp);
		if (access(cmde, X_OK | F_OK) == 0)
		{
			//free_tab(paths);
			execve(cmde, argss, NULL);
		}
		free(cmde);
	}
	free_tab(paths);
	free_tab(argss);
}

char	**lst_to_tab(t_lst **lst)
{
	int		i;
	int		len;
	char	**tab;
	t_lst	*cur;

	if (!lst || !*lst)
		return (NULL);

	len = list_len(lst);
	cur = *lst;
	tab = malloc(sizeof(char*) * (len + 1));
	if (!tab)
		return (NULL);

	i = 0;
	while (cur)
	{
		tab[i] = ft_strdup(cur->line);
		cur = cur->next;
		i++;
	}
	tab[i] = NULL;
	return (tab);
}


char	**arg_to_tab(t_arg *lst)
{
	int		i;
	char	**tab;
	t_arg	*cur;

	i = arg_len(&lst);
	tab = malloc(sizeof(char*) * (i + 1));
	i = 0;
	cur = lst;
	if (!tab)
		return(NULL);
	while (cur)
	{
		printf("la ligne est = %s\nle i est %i\n", cur->content, i);
		tab [i] =  ft_strdup(cur->content);
		cur = cur->next;
		i++;
	}
	tab [i] = NULL;
	return (tab);
}

int	arg_len(t_arg **ar)
{
	int	i;
	t_arg *cur;

	i = 0;
	cur = *ar;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	return (i);
}

void	cas(t_commandlist *mini)
{
	char	**argss;
	char	**env;

	if (*mini->cmd->args->content == '/')
	{
		env = lst_to_tab(mini->env);
		argss = arg_to_tab(mini->cmd->args);
		if (access(argss[0], X_OK | F_OK) == 0)
			execve(mini->cmd->args->content, argss, env);
		free_tab(argss);
		free_tab(env);
	}
}

int list_len(t_lst **lst)
{
    int i;
	t_lst *cur;

    i = 0;
	cur = *lst;
    while (cur)
    {
        i++;
        cur = cur ->next;
    }
    return (i);
}

int	lefork(void)
{
	int	i;

	i = fork();
	if (i < 0)
		return (perror("fork1"), -1);
	return (i);
}


void	red(t_command *cur)
{
	t_command *prev;
	t_command *next;

	prev = cur->prev;
	next = cur->next;
	
	if (prev && prev->pipe[0] != -1)
	{
		dup2(prev->pipe[0], STDIN_FILENO);
		close(prev->pipe[0]);
	}
	
	
	if (next && next->pipe[1] != -1)
	{
		dup2(next->pipe[1], STDOUT_FILENO);
		close(next->pipe[1]);
	}
	//printf("cur: %p, prev: %p, next: %p\n", (void *)cur, (void *)prev, (void *)next);

	her_doc(cur);
	in_file(cur);
	out_file(cur);
	add_file(cur);
}


void	in_file(t_command * cur)
{
	t_arg	*fil;
	int		fd;

	fil = cur ->infile;
	while (fil)
	{
		if (access(fil->content, F_OK) == -1)
		{
			printf("zsh: no such file or directory: %s \n", fil->content);
			return ;
		}
		if (access(fil->content, R_OK) == -1)
		{
			printf("zsh: permission denied: %s \n", fil->content);
			return ;
		}
		
		fd = open(fil->content, O_RDONLY, 0644);
		if (fd < 0)
		{
			printf("can't open %s \n", fil->content);
			return ;
		}
		dup2(fd, STDIN_FILENO);
		close (fd);
		fil = fil ->next;
	}
}

void	out_file(t_command * cur)
{
	t_arg	*fil;
	int		fd;

	fil = cur ->outfile;
	fd = -1;
	while (fil)
	{
		fd = open(fil->content, O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (fd < 0 || access(fil->content, R_OK | W_OK) == -1)
		{
			printf("can't open %s \n", fil->content);
			return ;
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		fil = fil ->next;
	}
}

void	add_file(t_command * cur)
{
	t_arg	*fil;
	int		fd;

	fil = cur ->append;
	fd = -1;
	while (fil)
	{
		fd = open(fil->content, O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0644);
		if (fd < 0 || access(fil->content, R_OK | W_OK) == -1)
		{
			printf("can't open %s \n", fil->content);
			return ;
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		fil = fil ->next;
	}
}

void	her_doc(t_command * cur)
{
	t_arg	*fil;
	char	*line;
	t_lst	**lst;

	lst = malloc(sizeof(t_lst *));
	fil = cur ->heredoc;
	while (fil)
	{
		while (1)
		{
			line = readline("user:");
			if (ft_strncmp(fil->content, line, ft_strlen(line)))
				break;
			append_node(NULL, line, lst);
			free(line);
		}
		redirect_lines(lst);
		clean_lst(lst);
		fil = fil->next;
	}
}
void	redirect_lines(t_lst **lst)
{
	int	pipes[2];
	t_lst *act;

	act = *lst;
	if (pipe(pipes) < 0)
		return ;
	while (act)
	{
		write(pipes[1], act->line, ft_strlen(act->line));
		write(pipes[1], "\n", 1);
		act = act->next;
	}
	dup2(pipes[0], STDIN_FILENO);
	close(pipes[0]);
	close(pipes[1]);
}

void	clean_lst(t_lst **lst)
{
	t_lst *cur;
	t_lst *next;

	cur = *lst;
	while (cur)
	{
		next = cur->next;
		free(cur->line);
		free(cur);
		cur = next;
	}
		free(lst);
}


int	ft_strnstri(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	a;

	i = 0;
	if (!*needle)
		return (-1);
	while (haystack[i] && i < len)
	{
		a = 0;
		while (haystack[i + a] && needle[a] && haystack[i + a] && len > i + a
			&& haystack[i + a] == needle[a])
			a++;
		if (!needle[a])
			return (1);
		i++;
	}
	return (-1);
}

void	free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab && tab[i])
		free(tab[i++]);
	free(tab);
	tab = NULL;
}