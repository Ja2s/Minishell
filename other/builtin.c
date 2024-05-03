/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:32:01 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/03 15:38:58 by jgavairo         ###   ########.fr       */
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

int	ft_builtins(t_cmd *lst, t_env *mini_env)
{
	int	i;
	int	builtins;
	char	*cwd;

	i = 1;
	if (ft_strcmp(lst->args[0], "pwd") == 0)
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
		{
			perror("getcwd");
			exit(EXIT_FAILURE);
		}
		printf("%s\n", cwd);
		free(cwd);
		builtins = 1;
	}
	else if (ft_strcmp(lst->args[0], "echo") == 0)
	{
		while (lst->args[i] && ft_strncmp(lst->args[i], "-n", 2) == 0)
				i++;
		while (lst->args[i])
		{
			printf("%s", lst->args[i]);
			if (lst->args[++i]) printf(" ");  // Ajoute un espace entre les arguments
		}
		if (lst->args[1] && ft_strncmp(lst->args[1], "-n", 2) != 0)	
			printf("\n");
		builtins = 1;
	}
	else if (ft_strcmp(lst->args[0], "env") == 0)
			env_cmd(mini_env);
	else
		builtins = 0;
	return builtins;
}

