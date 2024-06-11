/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_nd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:51:56 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/11 11:13:55 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		(*cmd)->delimiter = ft_calloc((*cmd)->nb_del + 1, sizeof (char *));
	if (!(*cmd)->delimiter && (*cmd)->nb_del > 0)
		return (-1);
	end_heredoc(pipes, cmd);
	return (0);
}

void	heredoc_copyer_init(t_cmd **cmd, t_int *var, int i)
{
	(*cmd)->heredoc = true;
	(*var).p = i + 1;
}

int	skip_space(t_int *var, char *pipes)
{
	while (pipes[(*var).p] && ft_isspace(pipes[(*var).p]) == 1)
		(*var).p++;
	return ((*var).p);
}

int	heredoc_copyer(char *pipes, t_cmd **cmd, int i, int del)
{
	t_int	var;

	init_var(&var);
	heredoc_copyer_init(cmd, &var, i);
	var.start = skip_space(&var, pipes);
	while (pipes[var.p] && ft_isspace(pipes[var.p]) == 0 && pipes[var.p] != '<')
		var.p++;
	(*cmd)->delimiter[del] = ft_substr(pipes, var.start, var.p - var.start);
	if ((*cmd)->delimiter[del] == NULL)
		return (-1);
	while ((*cmd)->delimiter[del][var.x])
	{
		if ((*cmd)->delimiter[del][var.x] == 34 || \
		(*cmd)->delimiter[del][var.x] == 39)
		{
			(*cmd)->delimiter[del] = heredoc_w_cote((*cmd)->delimiter[del]);
			if (!(*cmd)->delimiter[del])
				return (-1);
			(*cmd)->expand_heredoc = 0;
			break ;
		}
		(*cmd)->expand_heredoc = 1;
		var.x++;
	}
	return (var.p);
}
