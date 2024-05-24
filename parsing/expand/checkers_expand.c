/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:07:18 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/24 11:36:48 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	negative_checker_sp(char **rl)
{
	int		i;

	i = 0;
	doll_heredoc(&(*rl));
	while ((*rl)[i])
	{
		if (ft_isspace((*rl)[i]) == 1)
			(*rl)[i] = (*rl)[i] * -1;
		i++;
	}
}

void	in_cote_checker(t_expand **var, char *output, int i)
{
	while (i > 0)
	{
		if (output[i] == '"')
			(*var)->in_cote = (*var)->in_cote * -1;
		i--;
	}
}

void	in_redirection_checker(t_expand **var, char *output, int i)
{
	while (i >= 0)
	{
		if (output[i] == '>' || output[i] == '<')
			(*var)->in_redirecter = true;
		i--;
	}
}

int	space_in_value_checker(t_expand *var)
{
	int	i;

	i = 0;
	if (var->in_cote < 1 && var->in_redirecter == true)
	{
		while (var->value[i])
		{
			if (ft_isspace(var->value[i]) == 1)
				return (-1);
			i++;
		}
	}
	return (0);
}
