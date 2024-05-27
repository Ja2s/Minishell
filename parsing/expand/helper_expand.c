/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:19:19 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/27 11:43:05 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	negative_checker(char *rl)
{
	int		i;

	i = -1;
	doll_heredoc(&rl);
	while (rl[++i])
	{
		if (rl[i] && rl[i] == 34)
		{
			i++;
			while (rl[i] && rl[i] != 34)
			{
				double_negativer(i, &rl);
				i++;
			}
		}
		if (rl[i] && rl[i] == 39)
		{
			i++;
			while (rl[i] && rl[i] != 39)
			{
				simple_negativer(i, &rl);
				i++;
			}
		}
	}
}

int	expand_initializer(t_expand **var)
{
	(*var) = malloc(sizeof(t_expand));
	if (!*var)
		return (-1);
	(*var)->name_start = 0;
	(*var)->name_end = 0;
	(*var)->name_len = 0;
	(*var)->name = NULL;
	(*var)->value = NULL;
	(*var)->value_len = 0;
	(*var)->code_copy = 0;
	(*var)->nb_numbers = 0;
	(*var)->in_cote = -1;
	(*var)->in_redirecter = false;
	(*var)->output = NULL;
	(*var)->i = 0;
	(*var)->p = 0;
	(*var)->pos_doll = 0;
	return (0);
}

void	free_expand(t_expand **expand)
{
	if (!expand || !*expand)
		return ;
	if ((*expand)->name)
	{
		free((*expand)->name);
		(*expand)->name = NULL;
	}
	if ((*expand)->value)
	{
		free((*expand)->value);
		(*expand)->value = NULL;
	}
}

char	*del_doll(char *output, int i)
{
	int		x;
	char	*tmp;

	x = 0;
	tmp = ft_calloc(ft_strlen(output), sizeof (char));
	if (!tmp)
		return (NULL);
	while (x < i)
	{
		tmp[x] = output[x];
		x++;
	}
	i++;
	while (output[i])
	{
		tmp[x] = output[i];
		x++;
		i++;
	}
	return (free(output), tmp);
}
