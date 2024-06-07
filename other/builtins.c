/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:32:01 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/07 17:56:52 by jgavairo         ###   ########.fr       */
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
	char *tmp;
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
				return(tmp);
			}
		}
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

int swap_sort_env(char ***tab, int i)
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
			while ((*tab)[i][p] && (*tab)[i + 1][p] && (*tab)[i][p] == (*tab)[i + 1][p])
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
		while(tab[i])
			printf("declare -x %s\n", tab[i++]);
		free_pipes(tab);
	}
	return (0);
}

int	check_variable(t_env **mini_env, char *name, char *value)
{
	t_env *tmp;

	tmp = (*mini_env);
	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0)
		{
			if (value)
			{
				free(tmp->value);
				tmp->value = ft_strdup(value);
				if(!tmp->value)
					return (-1);
			}
			else
			{
				free(tmp->value);
				tmp->value = NULL;
			}
			return(1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	ft_isalpha_export(int c)
{
	if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) ||\
	c == '_' )
		return (1);
	else
		return (0);
}

int	spec_export(char *cmd)
{
	int i;

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
			return(0);
		if (ft_isalpha_export(cmd[i]) == 0 && ft_isdigit(cmd[i]) == 0)
			return(1);
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

void	exit_export(t_data *data, int i)
{
	exit_status_n_free(data, 1, "export: not a valid identifier: ");
	write(2, data->cmd->args[i], ft_strlen(data->cmd->args[i]));
	write(2,"\n", 1);
}

int	ft_export(t_data *data, t_env **mini_env, t_cmd *cmd)
{
	int		i;
	int		x;
	
	if (!cmd->args[1])
		ft_export_display(*mini_env);
	else
	{	
		i = 1;
		while (cmd->args[i])
		{
			if (spec_export(cmd->args[i]) == 0)
			{
				if (stock_variable(&data, i) == -1)
					return (-1);
			}
			else
				exit_export(data, i);
			i++;
		}
	}
	return (0);
}

int	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (-1);
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

void	ft_echo(t_cmd *lst)
{
	int	i;
	int	i_print;
	int	j;

	i = 1;
	i_print = 1;
	while (lst->args[i] && ft_strncmp(lst->args[i], "-n", 2) == 0)
	{
		j = 1;
		while (lst->args[i][j] == 'n')
			j++;
		if (lst->args[i][j] == '\0')
			i_print++;
		else
			break;
		i++;
	}
	i = i_print;
	while (lst->args[i])//affichage
	{
		printf("%s", lst->args[i]);
		if (lst->args[++i])
			printf(" ");// Ajoute un espace entre les arguments
	}
	if (i_print == 1)
		printf("\n");
	else if (!lst->args[1])
		printf("\n");
}

int	ft_builtins(t_cmd *lst)
{
	if (ft_strcmp(lst->args[0], "pwd") == 0)
	{
		if (ft_pwd() == -1)
		{
			perror("getcwd");
			exit(EXIT_FAILURE);
		}
		return (1);
	}
	else if (ft_strcmp(lst->args[0], "echo") == 0)
		return (ft_echo(lst), 1);
	return (0);
}
