/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_nd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:42:05 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/11 11:42:36 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	env_cmd(t_env *env)
{
	t_env	*mini_env;

	mini_env = env;
	while (mini_env)
	{
		if (mini_env->value)
			printf("%s=%s\n", mini_env->name, mini_env->value);
		mini_env = mini_env->next;
	}
}

char	*ft_getenv(char *name, t_env *mini_env)
{
	t_env	*env;
	char	*tmp;

	tmp = NULL;
	env = mini_env;
	while (env)
	{
		if (ft_strcmp(name, env->name) == 0)
		{
			if (env->value)
			{
				tmp = ft_strdup(env->value);
				return (tmp);
			}
			else
			{
				tmp = ft_strdup("");
				return (tmp);
			}
		}
		env = env->next;
	}
	return (NULL);
}

int	ft_copy_env(t_env **copy, t_env *mini_env)
{
	t_env	*new_elem;
	t_env	*tmp;

	tmp = NULL;
	tmp = mini_env;
	while (tmp)
	{
		new_elem = NULL;
		new_elem = malloc(sizeof (t_env));
		if (!new_elem)
			return (-1);
		new_elem->name = ft_strdup(tmp->name);
		if (!new_elem->name)
			return (-1);
		new_elem->value = ft_strdup(tmp->value);
		if (!new_elem->value)
			return (-1);
		ft_envadd_back(copy, new_elem);
		tmp = tmp->next;
	}
	return (0);
}

int	swap_sort_env(char ***tab, int i)
{
	char	*tmp;

	tmp = (*tab)[i + 1];
	(*tab)[i + 1] = (*tab)[i];
	(*tab)[i] = tmp;
	return (0);
}

void	sort_env(char ***tab)
{
	int		i;
	int		p;

	i = 0;
	while ((*tab)[i + 1])
	{
		p = 0;
		if ((*tab)[i][p] > (*tab)[i + 1][p])
			i = swap_sort_env(tab, i);
		else if ((*tab)[i][p] == (*tab)[i + 1][p])
		{
			while ((*tab)[i][p] && (*tab)[i + 1][p] && \
			(*tab)[i][p] == (*tab)[i + 1][p])
				p++;
			if ((*tab)[i][p] > (*tab)[i + 1][p])
				i = swap_sort_env(tab, i);
			else
				i++;
		}
		else
			i++;
	}
}
