/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:21:24 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/11 11:22:58 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	end_name_search(t_expand **var)
{
	while ((*var)->output[(*var)->name_end] && \
	ft_isspace((*var)->output[(*var)->name_end]) == 0 && \
	((ft_isalnum((*var)->output[(*var)->name_end], 0) == 1) || \
	(*var)->output[(*var)->name_end] == '_'))
		(*var)->name_end++;
}

void	expand_on_output(t_expand **var, char **rl)
{
	(*var)->i = 0;
	while ((*var)->i < ((*var)->name_start - 1))
	{
		(*rl)[(*var)->i] = (*var)->output[(*var)->i];
		(*var)->i++;
	}
	if ((*var)->value && (*var)->in_cote > 0)
		negative_checker_sp(&(*var)->value);
	(*var)->p = 0;
	if ((*var)->value)
	{
		while ((*var)->value[(*var)->p])
		{
			(*rl)[(*var)->i] = (*var)->value[(*var)->p];
			(*var)->i++;
			(*var)->p++;
		}
	}
	(*var)->p = (*var)->name_end;
	while ((*var)->output[(*var)->p])
	{
		(*rl)[(*var)->i] = (*var)->output[(*var)->p];
		(*var)->i++;
		(*var)->p++;
	}
}

int	basic_expander_helper(t_expand **var, t_data *data, char **rl)
{
	in_cote_checker(&(*var), (*var)->output, (*var)->i);
	in_redirection_checker(&(*var), (*var)->output, (*var)->i);
	if (!(*var))
		return (-1);
	expand_nd(var);
	end_name_search(var);
	(*var)->name_len = (*var)->name_end - (*var)->name_start;
	(*var)->name = ft_substr((*var)->output, \
	(*var)->name_start, (*var)->name_len);
	if (!(*var)->name)
		return (-1);
	(*var)->value = ft_getenv((*var)->name, data->mini_env);
	if ((*var)->value)
		negative_checker_variable(var, &data);
	*rl = ft_calloc(((*var)->value_len - (*var)->name_len + \
	ft_strlen((*var)->output) + 1), sizeof(char));
	if (!rl)
		return (-1);
	expand_on_output(var, rl);
	(*rl)[(*var)->i] = '\0';
	return (0);
}

int	basic_expander(char *rl, t_expand **var, t_data *data)
{
	char	*new_rl;

	new_rl = rl;
	if (basic_expander_helper(var, data, &new_rl) == -1)
	{
		free(new_rl);
		return (-1);
	}
	if (new_rl)
	{
		free((*var)->output);
		(*var)->output = ft_strdup(new_rl);
		free(new_rl);
	}
	if ((*var)->value && rl[0])
		command_positiver((*var)->value);
	if ((*var)->value_len > 0)
		(*var)->i = ((*var)->pos_doll + (*var)->value_len - 1);
	else
		(*var)->i = ((*var)->pos_doll + (*var)->value_len);
	return (0);
}

void	free_expand_opt(t_expand *var)
{
	if (var->name)
	{
		free(var->name);
		var->name = NULL;
	}
	if (var->value)
	{
		free(var->value);
		var->value = NULL;
	}
}
