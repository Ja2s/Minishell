/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exitcode_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:10:06 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/24 11:14:59 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exitcode_expander_init(t_expand **var, t_data **data, char **rl)
{
	if (!var)
		return (-1);
	(*var)->name_start = (*var)->i + 1;
	(*var)->name_end = (*var)->name_start + 1;
	(*var)->name_len = (*var)->name_end - (*var)->name_start;
	(*var)->name = ft_substr((*var)->output, \
	(*var)->name_start, (*var)->name_len);
	if (!(*var)->name)
		return (-1);
	(*var)->value = ft_itoa((*data)->exit_code);
	if ((*var)->value)
		(*var)->value_len = ft_strlen((*var)->value);
	*rl = ft_calloc(((*var)->value_len - (*var)->name_len + \
	ft_strlen((*var)->output) + 1), sizeof(char));
	if (!rl)
		return (-1);
	(*var)->i = 0;
	return (0);
}

int	exitcode_expander_finish(t_expand **var, char **rl)
{
	while ((*var)->output[(*var)->p])
	{
		(*rl)[(*var)->i] = (*var)->output[(*var)->p];
		(*var)->i++;
		(*var)->p++;
	}
	(*rl)[(*var)->i] = '\0';
	(*var)->output = ft_strdup(*rl);
	if (!(*var)->output)
		return (-1);
	(*var)->i = (*var)->pos_doll;
	return (0);
}

int	exitcode_expander(t_expand **var, t_data **data, char *rl)
{
	if (exitcode_expander_init(var, data, &rl) == -1)
		return (-1);
	while ((*var)->i < ((*var)->name_start - 1))
	{
		rl[(*var)->i] = (*var)->output[(*var)->i];
		(*var)->i++;
	}
	(*var)->p = 0;
	if ((*var)->value)
	{
		while ((*var)->value[(*var)->p])
		{
			rl[(*var)->i] = (*var)->value[(*var)->p];
			(*var)->i++;
			(*var)->p++;
		}
	}
	(*var)->p = (*var)->name_end;
	if (exitcode_expander_finish(var, &rl) == -1)
		return (-1);
	return (0);
}
