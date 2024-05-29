/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gavairon <gavairon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:58:25 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/29 16:06:31 by gavairon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*ft_envlast(t_env *lst)
{
	t_env	*tmp;

	if (!lst)
		return (NULL);
	tmp = lst;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	ft_envadd_back(t_env **env, t_env *new)
{
	if (*env)
		ft_envlast(*env)->next = new;
	else
		*env = new;
}

t_env	*env_new(void)
{
	t_env	*new_elem;

	new_elem = ft_calloc(1, sizeof (t_env));
	if (!new_elem)
		return (NULL);
	return (new_elem);
}

int	if_condition_expand(t_expand *var, int choice)
{
	if (choice == 1)
	{
		if (var->output)
		{
			if (condition_n_one(var) == 1)
				return (1);
		}
	}
	else if (choice == 2)
	{
		if (var->output)
		{
			if (condition_n_two(var) == 1)
				return (1);
			if (var->output[var->i] == '$' && var->output[var->i + 1] == '$')
			{
				var->i++;
				return (1);
			}
		}
	}
	return (0);
}

void	expand_nd(t_expand **var)
{
	(*var)->name_start = (*var)->i + 1;
	(*var)->pos_doll = (*var)->i;
	(*var)->name_end = (*var)->name_start;
}
