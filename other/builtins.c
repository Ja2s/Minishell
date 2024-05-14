/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:32:01 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/14 16:45:17 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	env_cmd(t_env *env)
{
	t_env	*mini_env;

	mini_env = env;
	while (mini_env)
	{
		if (!mini_env->name || !mini_env->value)
			mini_env = mini_env->next;
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

int ft_copy_env(t_env **copy, t_env *mini_env)
{
	t_env	*new_elem;
	t_env	*tmp;
	
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
		if(!new_elem->value)
			return (-1);
		ft_envadd_back(copy, new_elem);
		tmp = tmp->next;
	}
	return (0);
}

int	ft_export_display(t_env *mini_env)
{
	t_env	*copy;
	
	copy = malloc(sizeof(t_env));
	ft_copy_env(&copy, mini_env);
	env_cmd(copy);
	return (0);
}

int	ft_export(t_env **mini_env, t_cmd *cmd)
{
	t_env	*new_elem;
	int		i;
	char	**variable;
	if (!cmd->args[1])
		ft_export_display(*mini_env);
	else
	{	
		i = 0;
		variable = NULL;
		new_elem = malloc(sizeof(t_env));
		if (!new_elem)
			return (-1);
		variable = ft_split(cmd->args[1], '=');
		if (!variable || !variable[0] || !variable[1])
			return (free(new_elem), -1);
		new_elem->name = ft_strdup(variable[0]);
		new_elem->value = ft_strdup(variable[1]);
		ft_envadd_back(mini_env, new_elem);
		free(variable[0]);
		free(variable[1]);
		free(variable);
	}
	return (0);
}

int	ft_builtins(t_cmd *lst)
{
	int		i;
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
		return (1);
	}
	else if (ft_strcmp(lst->args[0], "echo") == 0)
	{
		while (lst->args[i] && ft_strncmp(lst->args[i], "-n", 2) == 0)
			i++;
		while (lst->args[i])
		{
			printf("%s", lst->args[i]);
			if (lst->args[++i])
				printf(" ");// Ajoute un espace entre les arguments
		}
		if (lst->args[1] && ft_strncmp(lst->args[1], "-n", 2) != 0)
			printf("\n");
		return (1);
	}
	return (0);
}
