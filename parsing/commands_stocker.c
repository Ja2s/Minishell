/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_stocker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gavairon <gavairon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 12:51:59 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/16 11:28:49 by gavairon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	pipes_counter(char *rl)
{
	int	i;

	i = 0;
	while (rl[i])
	{
		if (rl[i] == '|')
			return (1);
		i++;
	}
	return (0);
}

void	command_stocker(char **input, t_cmd **cmd)
{
	int	i;

	i = 0;
	while (input[i])
	{
		(*cmd)->args[i] = ft_strdup(input[i]);
		i++;
	}
	(*cmd)->args[i] = NULL;
}

int	args_memory_alloc(char **input, t_cmd **cmd)
{
	int	i;

	i = 0;
	while (input[i])
		i++;
	(*cmd)->nb_args = i;
	(*cmd)->args = malloc(sizeof(char *) * (i + 1));
	if (!(*cmd)->args)
		return (-1);
	return (0);
}

int	memory_alloc(char **input, t_cmd **cmd)
{
	if (args_memory_alloc(input, cmd) == -1)
		return (-1);
	return (0);
}

int	stock_input(char **input, t_cmd **cmd)
{
	if (memory_alloc(input, cmd) == -1)
		return (-1);
	command_stocker(input, cmd);
	return (0);
}
