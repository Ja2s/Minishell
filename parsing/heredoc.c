/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:38:50 by jgavairo          #+#    #+#             */
/*   Updated: 2024/04/30 13:01:28 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//compter le nb de heredoc pour sqvoir le nb de lignes dans le tab delimiter <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
int	heredoc_counter(char *pipes)
{
	int	i;
	int	heredoc;

	heredoc = 0;
	i = 0;
	while (pipes[i])
	{
		if (pipes[i] && pipes[i] == '<')
		{
			i++;
			if (pipes[i] && pipes[i] == '<')
			{
				heredoc++;
				i++;
			}
		}
		else
			i++;
	}
	return (heredoc);
}

int	heredoc_memory_allocer(char *pipes, t_cmd **cmd)
{
	(*cmd)->nb_del = heredoc_counter(pipes);
	if ((*cmd)->nb_del > 0)
		(*cmd)->delimiter = malloc(sizeof(char*)* (*cmd)->nb_del);
	if (!(*cmd)->delimiter && (*cmd)->nb_del > 0)
		return (-1);
	return (0);
}

int heredoc_copyer(char *pipes, t_cmd **cmd, int *i, int del)
{
	int start = 0;
	int p = *i;
	if (pipes[p] && pipes[p] == '<')
	{
		(*cmd)->heredoc = true;
		p++;
		while (pipes[p] && pipes[p] == ' ')
			p++;
		start = p;
		while (pipes[p] && pipes[p] != ' ')
			p++;
		(*cmd)->delimiter[del] = ft_substr(pipes, start, p - start);
		*i = p;	
	}
	return (0);
}

void	end_heredoc(char *pipes, t_cmd **cmd)
{
	size_t	i;

	i = 0;
	while(pipes[i])
	{
		if (pipes[i] == '<')
		{
			i++;
			if (pipes[i] == '<')
				(*cmd)->end_heredoc = 1;
			else
				(*cmd)->end_heredoc = 0;
		}
		i++;
	}
}

int	heredoc_checker(char *pipes, t_cmd **cmd)
{
	int i;
	int	del;
	
	del = 0;
	i = 0;
	if (heredoc_memory_allocer(pipes, cmd) == -1)
		return (-1);
	end_heredoc(pipes, cmd);
	//printf("End heredoc = %d\n", (*cmd)->end_heredoc);
	while (pipes[i])
	{
		if (pipes[i] && pipes[i] == '<')
		{
			i++;
			if (pipes[i] && pipes[i] == '<')
			{
				heredoc_copyer(pipes, cmd, &i, del);
				if (del < (*cmd)->nb_del)
					del++;
			}
		}
		else
			i++;
	}
	return (0);
}
