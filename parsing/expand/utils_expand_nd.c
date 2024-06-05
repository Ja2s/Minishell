/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_expand_nd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 17:15:12 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/05 11:25:38 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_isspace_neg(char c)
{
	if (c == -12 || c == -9 || c == -10 || c == -13 || c == -11 || c == -32)
		return (1);
	return (0);
}

int	condition_n_two(t_expand *var)
{
	if (var->output[var->i] == '$' && \
	(var->output[var->i + 1] == '\0' || \
	ft_isspace(var->output[var->i + 1]) == 1 || \
	ft_isspace_neg(var->output[var->i + 1]) == 1 || \
	var->output[var->i + 1] == ':' || \
	var->output[var->i + 1] == '='))
		return (1);
	else
		return (0);
}

int	condition_n_one(t_expand *var)
{
	if (var->output[var->i] && var->output[var->i] == '$' && \
	var->output[var->i + 1] && (var->output[var->i + 1] == 34 || \
	var->output[var->i + 1] == 39))
		return (1);
	else
		return (0);
}
