/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:38:50 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/24 16:51:46 by jgavairo         ###   ########.fr       */
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

int	heredoc_checker(char *pipes, t_cmd **cmd)
{
	int	i;
	int	del;

	del = 0;
	i = 0;
	if (heredoc_memory_allocer(pipes, cmd) == -1)
		return (-1);
	end_heredoc(pipes, cmd);
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
