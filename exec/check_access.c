/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:24:55 by rasamad           #+#    #+#             */
/*   Updated: 2024/06/11 22:26:52 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_check_slash_point(t_cmd *lst)
{
	int		i;

	i = 0;
	if (lst->args[0][0] == '.' && lst->args[0][1] == '\0')
		return (-1);
	while (lst->args[0][i])
	{
		if (lst->args[0][i] != '/' && lst->args[0][i] != '.')
			return (0);
		if (lst->args[0][i] == '.' && \
		lst->args[0][i + 1] == '.' && lst->args[0][i + 2] == '.')
			return (0);
		i++;
	}
	return (-2);
}

void	display_is_dir(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": Is a directory", 2);
	write(2, "\n", 1);
}

int	ft_check_slash_access(t_data *data, t_cmd *lst)
{
	int	check_slash_point;

	if (!lst->args[0])
		return (0);
	check_slash_point = ft_check_slash_point(lst);
	if (check_slash_point == -1)
		return (exit_status(data, 2, \
		"minishell: .: filename argument required\n"), -1);
	if (check_slash_point == -2)
		return (exit_status(data, 126, ""), display_is_dir(lst->args[0]), -1);
	if (ft_strchr(lst->args[0], '/'))
	{
		if (access(lst->args[0], F_OK) == -1)
			return (exit_status(data, 127, ""), \
			display_no_such(lst->args[0]), -1);
		else
			return (lst->path_cmd = ft_strdup(lst->args[0]), 0);
	}
	return (1);
}

int	ft_find_access(t_data *data, t_cmd *lst)
{
	while (access(lst->path_cmd, F_OK) == -1)
	{
		if (lst->split_path[lst->i] == NULL)
			return (exit_status(data, 127, ""), display_error_cmd(lst), -1);
		free(lst->path_cmd);
		lst->path_cmd = ft_strjoin(lst->split_path[lst->i++], lst->slash_cmd);
		if (!lst->path_cmd)
			return (exit_status(data, 1, "malloc error from [path_cmd]\n"), -1);
	}
	return (0);
}

int	ft_check_access(t_data *data, t_cmd *lst)
{
	int	check_slash_access;

	check_slash_access = ft_check_slash_access(data, lst);
	if (check_slash_access != 1)
		return (check_slash_access);
	lst->slash_cmd = ft_strjoin("/\0", lst->args[0]);
	if (!lst->slash_cmd)
		return (-1);
	lst->i = 0;
	while (data->var.mini_env[lst->i] && \
	ft_strncmp(data->var.mini_env[lst->i], "PATH=", 5))
		lst->i++;
	if (data->var.mini_env[lst->i] == NULL)
		return (exit_status(data, 127, ""), display_no_such(lst->args[0]), -1);
	lst->split_path = ft_split(data->var.mini_env[lst->i], ':');
	if (!lst->split_path)
		return (exit_status(data, 1, "malloc error from [split_path]\n"), -1);
	lst->i = 0;
	lst->path_cmd = ft_strjoin(lst->split_path[lst->i], lst->slash_cmd);
	if (!lst->path_cmd)
		return (exit_status(data, 1, "malloc error from [path_cmd]\n"), -1);
	if (ft_find_access(data, lst) != 0)
		return (-1);
	return (0);
}
