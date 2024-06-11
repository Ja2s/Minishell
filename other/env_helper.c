/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:33:59 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/11 14:44:55 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_len(t_env *mini_env, int choice)
{
	int	len;

	if (choice == 1)
	{
		if (mini_env->name && mini_env->value)
			len = (ft_strlen(mini_env->name) + \
			ft_strlen(mini_env->value) + 2);
		else
			len = (ft_strlen(mini_env->name) + 2);
	}
	if (choice == 2)
	{
		if (mini_env->name && mini_env->value)
			len = (ft_strlen(mini_env->name) + \
			ft_strlen(mini_env->value) + 4);
		else
			len = (ft_strlen(mini_env->name) + 4);
	}
	return (len);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->name)
		{
			free(env->name);
			env->name = NULL;
		}
		if (env->value)
		{
			free(env->value);
			env->value = NULL;
		}
		free(env);
		env = tmp;
	}
}

int	ft_builtins_env(t_cmd *lst, t_data *data, int i)
{
	if (!lst->args[0])
		return (0);
	if (ft_strcmp(lst->args[0], "export") == 0 && i == 1 && !lst->next)
		return (ft_export(data, &data->mini_env, lst), 1);
	else if (ft_strcmp(lst->args[0], "unset") == 0 && i == 1 && !lst->next)
		return (ft_unset(&data), 1);
	else if (ft_strcmp(lst->args[0], "env") == 0 && i == 1 && !lst->next)
		return (env_cmd(data->mini_env), 1);
	else if (ft_strcmp(lst->args[0], "cd") == 0 && i == 1 && !lst->next)
		return (ft_cd(data), 1);
	return (0);
}
