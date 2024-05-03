/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:32:01 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/03 11:31:09 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	env_cmd(t_env *env)
{
	t_env	*mini_env;

	mini_env = env;
	while(mini_env)
	{
		printf("%s=%s\n", mini_env->name, mini_env->value);
		mini_env = mini_env->next;
	}
}

char	*ft_getenv(char *name, t_env *mini_env)
{
	t_env	*env;

	env = mini_env;
	while (env)
	{
		if (ft_strcmp(name, env->name) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}