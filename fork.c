/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gavairon <gavairon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 15:12:43 by rasamad           #+#    #+#             */
/*   Updated: 2024/04/19 15:07:47 by gavairon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

int	ft_first_fork(t_cmd *elem, t_struct **var, char **envp)
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
		if (elem->redirecter && elem->fd_infile > 0)// < f1
		{
			if (dup2(elem->fd_infile, STDIN_FILENO) == -1)
			{
				perror("dup2 first fd_infile failed : ");
				exit(EXIT_FAILURE);
			}
		}
		//envoi dans fd_outfile en prioriter s'il y en a un
		if (elem->redirecter && elem->fd_outfile > 0)// > f2
		{
			if (dup2(elem->fd_outfile, STDOUT_FILENO) == -1)
			{
				perror("dup2 first fd_outfile failed : ");
				exit(EXIT_FAILURE);
			}
		}
		//sinon dans pipe[1] si il y a un next, sinon dans stdout
		else if (elem->next) // --> | cmd
		{
			if (dup2((*var)->pipe_fd[1], STDOUT_FILENO) == -1)
			{
				perror("dup2 first pipe[1] failed : ");
				exit(EXIT_FAILURE);
			}
		}
		//ft_close(elem);
		execve(elem->path_cmd, elem->args, envp);
		perror("execve 1 : failed ");
		exit(0);
	}
	else if (pid > 0)
	{
		int	status;
		wait(&status);
		//printf("je suis dans le processus du parent du 1er fork, le pid de l'enfant est %d\n", pid);
	}
	return (0);
}

int	ft_middle_fork(t_cmd *elem, t_struct **var, char **envp)
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
		if (elem->redirecter && elem->fd_infile > 0)
		{
			if (dup2(elem->fd_infile, STDIN_FILENO))
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
		if (elem->redirecter && elem->fd_outfile > 0)
		{
			if (dup2(elem->fd_outfile, STDOUT_FILENO))
			{
				perror("dup2 middle fd_outfile failed :");
				exit(EXIT_FAILURE);
			}
		}
		//sinon dans pipe[1]
		else if (elem->next)
		{
			if (dup2((*var)->pipe_fd[1], STDOUT_FILENO) == -1)
			{
				perror("dup2 middle pipe[1] failed :");
				exit(EXIT_FAILURE);
			}
		}
		execve(elem->path_cmd, elem->args, envp);
		perror("execve 2 failed : ");
		//free elem->path et args
		exit(0);
	}
	else
	{
		int	status;
		wait(&status);
	}
	return (0);
}

int	ft_last_fork(t_cmd *elem, t_struct **var, char **envp)
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
		if (elem->redirecter && elem->fd_infile > 0)// < f1
		{
			if (dup2(elem->fd_infile, STDIN_FILENO) == -1)
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
		if (elem->redirecter && elem->fd_outfile > 0)// > f2
		{
			if (dup2(elem->fd_outfile, STDOUT_FILENO) == -1)
			{
				perror("dup2 last fd_outfile failed : ");
				exit(EXIT_FAILURE);
			}
		}
		execve(elem->path_cmd, elem->args, envp);
		perror("execve 3 : failed ");
		exit(0);
	}
	else if (pid > 0)
	{
		int	status;
		wait(&status);
		//printf("je suis dans le processus du parent du dernier fork, le pid de l'enfant est %d\n", pid);
		close((*var)->pipe_fd[0]);
	}
	return (0);
}