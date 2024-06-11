/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:17:49 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/11 11:21:45 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_expand(t_expand *var)
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
	if (var->output)
	{
		free(var->output);
		var->output = NULL;
	}
}

int	expand_basic(char *rl, t_expand *var, t_data *data)
{
	if (basic_expander(rl, &var, data) == -1)
		return (-1);
	free_expand_opt(var);
	return (0);
}

int	expand_exitcode(char *rl, t_expand *var, t_data *data)
{
	if (exitcode_expander(&var, &data, rl) == -1)
		return (-1);
	free_expand_opt(var);
	return (0);
}

void	dolls_expander_finisher(t_expand **var, char **result)
{
	*result = ft_strdup((*var)->output);
	free_expand((*var));
	free((*var));
	(*var) = NULL;
}

char	*dolls_expander(char *rl, t_data *data)
{
	t_expand	*var;
	char		*result;

	if (expand_initializer(&var) == -1)
		return (NULL);
	if (rl)
		var->output = ft_strdup(rl);
	if (!var->output)
		return (NULL);
	while (var->i < (int)ft_strlen(var->output))
	{
		var->value_len = 0;
		if (if_condition_expand(var, 1) == 1)
			var->output = del_doll(var->output, var->i);
		else if (if_condition_expand(var, 2) == 1)
			var->i++;
		else if (var->output[var->i] == '$' && var->output[var->i + 1] != '?')
			expand_basic(rl, var, data);
		else if (var->output[var->i] == '$' && var->output[var->i + 1] == '?')
			expand_exitcode(rl, var, data);
		else
			var->i++;
	}
	dolls_expander_finisher(&var, &result);
	return (free(rl), result);
}
