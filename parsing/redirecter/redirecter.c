/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirecter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:27:51 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/10 14:25:32 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	redirect_counter(char *pipes)
{
	int	i;
	int	len;

	len = 0;
	i = 0;
	while (pipes[i])
	{
		if (pipes[i] == '<' || pipes[i] == '>')
		{
			if (pipes[i] == '<' && pipes[i + 1] == '<')
				i = i + 2;
			else
			{
				len++;
				i++;
				if (pipes[i] == '<' || pipes[i] == '>')
					i++;
			}
		}
		else
			i++;
	}
	return (len);
}

static int	skip_spaces(char *pipes, int i)
{
	while (pipes[i] && ft_isspace(pipes[i]) == 1)
		i++;
	return (i);
}

int	redirecter(char *pipes, t_cmd **cmd)
{
	t_int	var;

	init_var(&var);
	var.len = redirect_counter(pipes);
	(*cmd)->nb_red = var.len;
	if (var.len > 0)
	{
		(*cmd)->redirecter = malloc(sizeof (char *) * (var.len + 1));
		if (!(*cmd)->redirecter)
			return (-1);
		var.len = 0;
		while (pipes[var.i])
		{
			if (pipes[var.i] == '<' || pipes[var.i] == '>')
			{
				if (redirecter_helper(pipes, cmd, &var) == -1)
					return (-1);
			}
			else
				var.i++;
		}
		(*cmd)->redirecter[var.x] = NULL;
	}
	return (0);
}

int	len_calculator(char	*pipes)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	while (pipes[i])
	{
		while ((pipes[i]) && (pipes[i] != '<' && pipes[i] != '>'))
		{
			x++;
			i++;
		}
		if ((pipes[i]) && (pipes[i] == '<' || pipes[i] == '>'))
		{
			i++;
			if (pipes[i] == '<' || pipes[i] == '>')
				i++;
			while (pipes[i] && ft_isspace(pipes[i]) == 1)
				i++;
			while (pipes[i] && ft_isspace(pipes[i]) == 0 && \
			pipes[i] != '<' && pipes[i] != '>')
				i++;
		}
	}
	return (x);
}
