/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:38:50 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/24 17:52:15 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	end_heredoc(char *pipes, t_cmd **cmd)
{
	size_t	i;

	i = 0;
	while (pipes[i])
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

void	init_var(t_int *var)
{
	var->p = 0;
	var->start = 0;
	var->x = 0;
	var->len = 0;
	var->i = 0;
}

int	heredoc_checker(char *pipes, t_cmd **cmd)
{
	int	i;
	int	del;

	del = 0;
	i = 0;
	if (heredoc_memory_allocer(pipes, cmd) == -1)
		return (-1);
	while (pipes[i])
	{
		if (pipes[i] && pipes[i] == '<')
		{
			i++;
			if (pipes[i] && pipes[i] == '<')
			{
				i = heredoc_copyer(pipes, cmd, i, del);
				if (i == -1)
					return (-1);
				if (del < (*cmd)->nb_del)
					del++;
			}
		}
		else
			i++;
	}
	return (0);
}
