/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:32:01 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/11 19:00:22 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exit_export(t_data *data, int i)
{
	exit_status_n_free(data, 1, "export: not a valid identifier: ");
	write(2, data->cmd->args[i], ft_strlen(data->cmd->args[i]));
	write(2, "\n", 1);
}

int	ft_export(t_data *data, t_env **mini_env, t_cmd *cmd)
{
	int		i;

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
			break ;
		i++;
	}
	i = i_print;
	while (lst->args[i])
	{
		printf("%s", lst->args[i]);
		if (lst->args[++i])
			printf(" ");
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
