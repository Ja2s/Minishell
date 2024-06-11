/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:47:52 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/11 14:45:52 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_unset(t_data **data)
{
	t_env	*tmp;
	t_env	*swap;
	int		i;

	i = 1;
	tmp = (*data)->mini_env;
	while ((*data)->cmd->args[i])
	{
		if (spec_export((*data)->cmd->args[i]) == 0)
		{
			while (tmp->next && ft_strcmp(tmp->next->name, (*data)->cmd->args[i]) != 0)
				tmp = tmp->next;
			if (tmp && tmp->next && ft_strcmp(tmp->next->name, (*data)->cmd->args[i]) == 0)
			{
				swap = tmp->next;
				tmp->next = tmp->next->next;
				free(swap->name);
				free(swap->value);
				free(swap);
			}
		}
		else
		{
			exit_status_n_free((*data), 1, "unset: not a valid identifier: ");
			write(2, (*data)->cmd->args[i], ft_strlen((*data)->cmd->args[i]));
			write(2,"\n", 1);
		}
		i++;
	}
}

int	ft_cd(t_data *data)
{
	char	*old_pwd;
	char	*pwd;
	int		i;

	i = 0;
	old_pwd = getcwd(NULL, 0);
	if (data->cmd->args[1] && ft_strcmp(data->cmd->args[1], "-") == 0)
	{
		while (data->var.mini_env[i] && ft_strncmp(data->var.mini_env[i], "OLDPWD=", 7) != 0)
			i++;
		if (!data->var.mini_env[i])
			return (free(old_pwd), display_error_cmd(data->cmd), -1);
		if (chdir(ft_strchr(data->var.mini_env[i], '/')) == -1)
		{
			perror("chdir OLDPWD failed ");
			return (free(old_pwd), -1);
		}
	}
	else if (!data->cmd->args[1])
	{
		while (data->var.mini_env[i] && ft_strncmp(data->var.mini_env[i], "HOME=", 5) != 0)
			i++;
		if (!data->var.mini_env[i])
			return (display_error_cmd(data->cmd), -1);
		if (chdir(ft_strchr(data->var.mini_env[i], '/')) == -1)
		{
			perror("chdir home failed ");
			return (free(old_pwd), -1);
		}
	}
	else if (chdir(data->cmd->args[1]) == -1)
	{
		perror("chdir failed ");
		return (free(old_pwd), -1);
	}
	check_variable(&data->mini_env, "OLDPWD", old_pwd);
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (-1);
	check_variable(&data->mini_env, "PWD", pwd);
	return (free(old_pwd), free(pwd), 0);
}

int	ft_is_builtins_no_access(t_cmd *lst)
{
	if (!lst->args[0])
		return (1);
	if (ft_strcmp(lst->args[0], "export") == 0)
		return (1);
	if (ft_strcmp(lst->args[0], "exit") == 0)
		return (1);
	else if (ft_strcmp(lst->args[0], "unset") == 0)
		return (1);
	else if (ft_strcmp(lst->args[0], "cd") == 0)
		return (1);
	return (0);
}
