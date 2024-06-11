/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_rd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:42:03 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/11 11:43:32 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_export_display(t_env *mini_env)
{
	int		i;
	char	**tab;

	if (mini_env)
	{
		i = 0;
		tab = ft_list_to_tab_cote(mini_env);
		if (!tab)
			return (-1);
		sort_env(&tab);
		while (tab[i])
			printf("declare -x %s\n", tab[i++]);
		free_pipes(tab);
	}
	return (0);
}

int	check_variable(t_env **mini_env, char *name, char *value)
{
	t_env	*tmp;

	tmp = (*mini_env);
	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0)
		{
			if (value)
			{
				free(tmp->value);
				tmp->value = ft_strdup(value);
				if (!tmp->value)
					return (-1);
			}
			else
			{
				free(tmp->value);
				tmp->value = NULL;
			}
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	ft_isalpha_export(int c)
{
	if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || \
	c == '_' )
		return (1);
	else
		return (0);
}

int	spec_export(char *cmd)
{
	int	i;

	i = 0;
	if (!cmd[0])
		return (1);
	if (cmd[0] >= '1' && cmd[0] <= '9')
		return (1);
	if (cmd[0] == '=')
		return (1);
	while (cmd[i])
	{
		if (i > 0 && cmd[i] == '=')
			return (0);
		if (ft_isalpha_export(cmd[i]) == 0 && ft_isdigit(cmd[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	stock_variable(t_data **data, int i)
{
	t_env	*new_elem;
	char	**variable;

	variable = ft_split((*data)->cmd->args[i], '=');
	if (!variable || !variable[0])
		return (new_elem = NULL, -1);
	if (check_variable(&(*data)->mini_env, variable[0], variable[1]) == 0)
	{
		new_elem = env_new();
		if (!new_elem)
			return (free_pipes(variable), -1);
		new_elem->name = ft_strdup(variable[0]);
		if (variable[1])
			new_elem->value = ft_strdup(variable[1]);
		ft_envadd_back(&(*data)->mini_env, new_elem);
	}
	free_pipes(variable);
	return (0);
}
