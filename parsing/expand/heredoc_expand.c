/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:24:51 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/07 18:07:34 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	doll_heredoc_helper(char **rl, int i)
{
	int	x;

	x = '$' * -1;
	if ((*rl)[i] == '$')
	{
		if (((*rl)[i + 1] == 34 || (*rl)[i + 1] == 39) && (*rl)[i - 1] != x)
		{
			ft_memmove(&(*rl)[i], &(*rl)[i + 1], strlen((*rl)) - i);
			i--;
		}
		else
			(*rl)[i] = (*rl)[i] * -1;
	}
	else
		i++;
	return (i);
}

void	doll_heredoc(char **rl)
{
	int	i;

	i = 0;
	while ((*rl)[i])
	{
		if ((*rl)[i] == '<')
		{
			i++;
			if ((*rl)[i] == '<')
			{
				i++;
				while (ft_isspace((*rl)[i]) == 1 || (*rl)[i] == 39 || (*rl)[i] == 34)
					i++;
				while ((*rl)[i])
					i = doll_heredoc_helper(rl, i);
			}
		}
		else
			i++;
	}
}

void	double_negativer(int i, char **rl)
{
	if (ft_isspace((*rl)[i]) == 1 || (*rl)[i] == '<' || \
	(*rl)[i] == '>' || (*rl)[i] == 39 || (*rl)[i] == '|')
		(*rl)[i] = (*rl)[i] * -1;
}

void	simple_negativer(int i, char **rl)
{
	if (ft_isspace((*rl)[i]) == 1 || (*rl)[i] == '$' || (*rl)[i] == '<' || \
	(*rl)[i] == '>' || (*rl)[i] == '|' || (*rl)[i] == 34)
		(*rl)[i] = (*rl)[i] * -1;
}
