/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:23:31 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/10 17:06:48 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*value_extractor(char *env)
{
	int		i;
	int		x;
	int		len;
	char	*str;

	len = 0;
	x = 0;
	i = 0;
	str = NULL;
	while (env[i] && env[i] != '=')
		i++;
	i++;
	x = i;
	while (env[i])
	{
		i++;
		len++;
	}
	str = ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (env[x])
		str[i++] = env[x++];
	return (str);
}

char	*name_extractor(char *env)
{
	int		i;
	char	*str;

	str = NULL;
	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	str = ft_calloc(i + 1, sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (env[i] && env[i] != '=')
	{
		str[i] = env[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*env_extractor(char	*env, int choice)
{
	int		i;
	int		x;
	int		len;
	char	*str;

	len = 0;
	x = 0;
	i = 0;
	str = NULL;
	if (choice == 1)
		str = name_extractor(env);
	else if (choice == 2)
		str = value_extractor(env);
	if (str == NULL)
		return (NULL);
	return (str);
}

int	env_copyer_nd(t_data **data, char **envp, t_env *tmp, int i)
{
	tmp->name = env_extractor(envp[i], 1);
	if (!tmp->name)
		return (free_env(tmp), exit_status((*data), 1, \
		"\033[38;5;214mMalloc error from [env_extractor]\n\033[0m"), -1);
	tmp->value = env_extractor(envp[i], 2);
	if (!tmp->value)
		return (free_env(tmp), exit_status((*data), 1, \
		"\033[38;5;214mMalloc error from [env_extractor]\n\033[0m"), -1);
	return (0);
}

int	env_copyer(t_data **data, char **envp, t_env **mini_env)
{
	int		i;
	t_env	*tmp;

	i = 0;
	if (!envp[i])
		printf("Info : Minishell has no environment\n");
	else
	{
		while (envp[i])
		{
			tmp = env_new();
			if (!tmp)
				return (exit_status((*data), 1, \
				"\033[38;5;214mMalloc error from [env_copyer]\n\033[0m"), -1);
			else
			{
				if (env_copyer_nd(data, envp, tmp, i) == -1)
					return (-1);
				ft_envadd_back(mini_env, tmp);
				i++;
			}
		}
	}
	return (0);
}
