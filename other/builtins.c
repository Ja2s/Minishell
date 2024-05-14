/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gavairon <gavairon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:32:01 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/14 20:42:41 by gavairon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	env_cmd(t_env *env)
{
	t_env	*mini_env;

	mini_env = env;
	while (mini_env)
	{
		printf("%s=%s\n", mini_env->name, mini_env->value);
		mini_env = mini_env->next;
		//free la liste si cest copy dans le cas de export et non de minin env (if choice == ...)
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
		if(!new_elem->value)
			return (-1);
		ft_envadd_back(copy, new_elem);
		tmp = tmp->next;
	}
	return (0);
}

void	sort_env(char ***tab)
{
	int		i;
	int		p;
	char	*tmp;

	i = 0;
	while ((*tab)[i + 1])
	{
		p = 0;
		if ((*tab)[i][p] > (*tab)[i + 1][p])
		{
			tmp = (*tab)[i + 1];
			(*tab)[i + 1] = (*tab)[i];
			(*tab)[i] = tmp;
			i = 0;
		}
		else if ((*tab)[i][p] == (*tab)[i + 1][p])
		{
			while ((*tab)[i][p] && (*tab)[i + 1][p] && (*tab)[i][p] == (*tab)[i + 1][p])
				p++;
			if ((*tab)[i][p] > (*tab)[i + 1][p])
			{
				tmp = (*tab)[i + 1];
				(*tab)[i + 1] = (*tab)[i];
				(*tab)[i] = tmp;
				i = 0;
			}
			else
				i++;
		}
		else
			i++;
	}
}


int	ft_export_display(t_env *mini_env)
{
	int		i;
	char	**tab;

	i = 0;
	tab = ft_list_to_tab_cote(mini_env);
	if (!tab)
		return (-1);
	sort_env(&tab);
	while(tab[i])
		printf("declare -x %s\n", tab[i++]);
	free_pipes(tab);
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
		if (!variable || !variable[0])
			return (free(new_elem), -1);
		new_elem->name = ft_strdup(variable[0]);
		if (variable[1])
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
