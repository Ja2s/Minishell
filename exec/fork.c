/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 15:12:43 by rasamad           #+#    #+#             */
/*   Updated: 2024/05/03 15:32:36 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_builtins(t_cmd *lst, t_env *mini_env)
{
	int	i;
	int	builtins;
	char	*cwd;

	i = 1;
	if (ft_strcmp(lst->args[0], "pwd") == 0)
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
		{
			perror("getcwd");
			exit(EXIT_FAILURE);
		}
		printf("%s\n", cwd);
		free(cwd);
		builtins = 1;
	}
	else if (ft_strcmp(lst->args[0], "echo") == 0)
	{
		while (lst->args[i] && ft_strncmp(lst->args[i], "-n", 2) == 0)
				i++;
		while (lst->args[i])
		{
			printf("%s", lst->args[i]);
			if (lst->args[++i]) printf(" ");  // Ajoute un espace entre les arguments
		}
		if (lst->args[1] && ft_strncmp(lst->args[1], "-n", 2) != 0)	
			printf("\n");
		builtins = 1;
	}
	else if (ft_strcmp(lst->args[0], "env") == 0)
			env_cmd(mini_env);
	else
		builtins = 0;
	return builtins;
}

char	**ft_cp_heredoc(t_cmd *lst)
{
	size_t		i;
	size_t		j;
	char	**tab;

	i = 0;
	j = 0;
	tab = NULL;
	while (lst->heredoc_content && lst->heredoc_content[i])
		i++;
	while (lst->args && lst->args[j])
		j++;
	tab = malloc(sizeof(char*) * (i + j + 1));
	i = 0;
	j = 0;
	while (lst->args[j])
	{
		tab[j] = ft_strdup(lst->args[j]);
		j++;
	}
	while (lst->heredoc_content[i])
	{
		tab[j] = ft_strdup(lst->heredoc_content[i]);
		i++;
	}
	tab[i + j] = NULL;
	return (tab);
}
/*
void	ft_display_arg(t_cmd *lst)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (lst != NULL)
	{
		printf("arg %d :\n", i);
		i++;
		while (lst->args && lst->args[j])
		{
			printf("%d|%s|\n", j, lst->args[j]);
			j++;
		}
	j = 0;
	lst = lst->next;
	}
}*/

char	*line_extractor(t_env *mini_env)
{
	char	*line;
	int		len;
	int		i;

	i = 0;
	len = 0;
	line = NULL;
	len = (ft_strlen(mini_env->name) + ft_strlen(mini_env->value) + 2);
	line = malloc(sizeof(char)* len);
	if (!line)
		return (NULL);
	len = 0;
	while (mini_env->name[len])
	{
		line[len] = mini_env->name[len];
		len++;
	}
	line[len++] = '=';
	while (mini_env->value[i])
		line[len++] = mini_env->value[i++];
	line[len] = '\0';
	return (line);
}

int	ft_envsize(t_env *mini_env)
{
	int	i;

	i = 0;
	while(mini_env)
	{
		i++;
		mini_env = mini_env->next;
	}
	return (i);
}

char	**ft_list_to_tab(t_env *mini_env)
{
	char	**tab;
	t_env	*tmp;
	int		len;
	int		i;

	i = 0;
	tab = NULL;
	tmp = mini_env;
	len = ft_envsize(mini_env);
	tab = ft_calloc(len, sizeof(char*));
	if(!tab)
		return (NULL);
	while (tmp)
	{
		tab[i++] = line_extractor(tmp);
		tmp = tmp->next;
	}
	return (tab);
}

int	ft_first_fork(t_cmd *lst, t_struct **var, t_env	*mini_env, char	**tab_mini_env)
{
	pid_t	pid;
	
	pid = fork();
	if (pid < 0)
	{
		perror("fork first failed :");	
		exit(1);
	}
	if (pid == 0)
	{
		//prend dans fd_infile en prioriter sinon dans stdin
		if (lst->redirecter && lst->fd_infile > 0)// < f1
		{
			if (dup2(lst->fd_infile, STDIN_FILENO) == -1)
			{
				perror("dup2 first fd_infile failed : ");
				exit(EXIT_FAILURE);
			}
		}
		//envoi dans fd_outfile en prioriter s'il y en a un
		if (lst->redirecter && lst->fd_outfile > 0)// > f2
		{
			if (dup2(lst->fd_outfile, STDOUT_FILENO) == -1)
			{
				perror("dup2 first fd_outfile failed : ");
				exit(EXIT_FAILURE);
			}
		}
		//sinon dans pipe[1] si il y a un next, sinon dans stdout
		else if (lst->next) // --> | cmd
		{
			if (dup2((*var)->pipe_fd[1], STDOUT_FILENO) == -1)
			{
				perror("dup2 first pipe[1] failed : ");
				exit(EXIT_FAILURE);
			}
		}
		//ft_close(lst);
		if (ft_builtins(lst, mini_env) != 0)
			exit(0);
		/*if (lst->heredoc_content && lst->end_heredoc == 1)
		{
			printf("ARG AV \n");
			ft_display_arg(lst);
			lst->args = ft_cp_heredoc(lst);
			printf("\n\nARG AP \n");
			ft_display_arg(lst);
			exit(0);
		}*/
		execve(lst->path_cmd, lst->args, tab_mini_env);
		perror("execve 1 : failed ");
		exit(0);
	}
	else if (pid > 0)
	{
		int	status;
		wait(&status);
	}
	return (0);
}

int	ft_middle_fork(t_cmd *lst, t_struct **var, char **tab_mini_env)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork middle failed :");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		//prend soit dans fd_infile en prioriter sil y en a un
		if (lst->redirecter && lst->fd_infile > 0)
		{
			if (dup2(lst->fd_infile, STDIN_FILENO))
			{
				perror("dup2 middle fd_indile failed :");
				exit(EXIT_FAILURE);
			}
		}
		//soit dans pipe[0]
		else
		{
			if (dup2((*var)->save_pipe, STDIN_FILENO))
			{
				perror("dup2 middle pipe_fd[0] failed :");
				exit(EXIT_FAILURE);
			}
		}
		//envoi dans fd_outfile en prioriter sil y en a un
		if (lst->redirecter && lst->fd_outfile > 0)
		{
			if (dup2(lst->fd_outfile, STDOUT_FILENO))
			{
				perror("dup2 middle fd_outfile failed :");
				exit(EXIT_FAILURE);
			}
		}
		//sinon dans pipe[1]
		else if (lst->next)
		{
			if (dup2((*var)->pipe_fd[1], STDOUT_FILENO) == -1)
			{
				perror("dup2 middle pipe[1] failed :");
				exit(EXIT_FAILURE);
			}
		}
		execve(lst->path_cmd, lst->args, tab_mini_env);
		perror("execve 2 failed : ");
		//free lst->path et args
		exit(0);
	}
	else
	{
		int	status;
		wait(&status);
	}
	return (0);
}

int	ft_last_fork(t_cmd *lst, t_struct **var, char **tab_mini_env)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0){
		perror("fork last failed :");	
		exit(1);
	}
	if (pid == 0)
	{
		//prend soit dans infile en prioriter sil y en a un
		if (lst->redirecter && lst->fd_infile > 0)// < f1
		{
			if (dup2(lst->fd_infile, STDIN_FILENO) == -1)
			{
				perror("dup2 last fd_infile failed : ");
				exit(EXIT_FAILURE);
			}
		}
		//soit dans pipe[0]
		else
		{
			if (dup2((*var)->save_pipe, STDIN_FILENO) == -1) // sortie de l'ancienne cmd
			{
				perror("dup2 last pipe_fd[0] failed : ");
				exit(EXIT_FAILURE);
			}
		}
		//envoi soit dans fd_outfile s'il y en a un soit dans stdout qui reste inchange (pas de dup2)
		if (lst->redirecter && lst->fd_outfile > 0)// > f2
		{
			if (dup2(lst->fd_outfile, STDOUT_FILENO) == -1)
			{
				perror("dup2 last fd_outfile failed : ");
				exit(EXIT_FAILURE);
			}
		}
		execve(lst->path_cmd, lst->args, tab_mini_env);
		perror("execve 3 : failed ");
		exit(0);
	}
	else if (pid > 0)
	{
		int	status;
		wait(&status);
		close((*var)->pipe_fd[0]);
	}
	return (0);
}