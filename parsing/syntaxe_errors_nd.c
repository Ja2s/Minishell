/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxe_errors_nd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:38:52 by gavairon          #+#    #+#             */
/*   Updated: 2024/05/24 17:02:22 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	starter_pipe(char *rl)
{
	if (rl[0] == '|')
		return (-1);
	return (0);
}

int	rafter_checker_one(char *rl, int p)
{
	int	i;

	i = p;
	if (rl[i] == 34)
	{
		i++;
		while (rl[i] != 34)
			i++;
	}
	return (i);
}

int	rafter_checker_two(char *rl, int p)
{
	int	i;

	i = p;
	if (rl[i] == 39)
	{
		i++;
		while (rl[i] != 39)
			i++;
	}
	return (i);
}

int	skip_cote(char *rl, int i, int choice)
{
	if (choice == 2)
	{
		i++;
		while (rl[i] != 34)
			i++;
	}
	else if (choice == 1)
	{
		i++;
		while (rl[i] != 39)
			i++;
	}
	return (i);
}

int	rafters_checker(char *rl)
{
	int	i;

	i = 0;
	while (rl[i])
	{
		i = rafter_checker_one(rl, i);
		i = rafter_checker_two(rl, i);
		if (rl[i] == '<' || rl[i] == '>')
		{
			i++;
			if (rl[i] == '<' || rl[i] == '>')
				i++;
			while (rl[i] == ' ')
				i++;
			if (rl[i] == '\0' || rl[i] == '<' || rl[i] == '>' || rl[i] == '|')
				return (-1);
		}
		else
			i++;
	}
	return (0);
}
