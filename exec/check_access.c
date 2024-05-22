/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:24:55 by rasamad           #+#    #+#             */
/*   Updated: 2024/05/14 13:57:50 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//Fonction	: Verifie si la args[0] est good
//Renvoi	: -2 --> malloc crash / -1 --> echec / 0 --> success 
int	ft_check_access(t_data *data)
{
	t_cmd *lst;

	lst = data->cmd;
	if (ft_strchr(lst->args[0], '/'))//verif si path de args[0]
	{
		if (access(lst->args[0], F_OK) == -1)
			return (display_no_such(lst), -1);
		else
		{
			lst->path_cmd = lst->args[0];
			return (printf("command path |%s| ok\n", lst->args[0]), 0);
		}
	}
	lst->slash_cmd = ft_strjoin("/\0", lst->args[0]);//ajout du slash au debut de la args[0] "/cat"
	if (!lst->slash_cmd)
		return (-2);
	lst->i = 0;
	if (lst->args[0])
	while (ft_strncmp(data->var.mini_env[lst->i], "PATH=", 5))
		lst->i++;
	lst->split_path = ft_split(data->var.mini_env[lst->i], ':');
	if (!lst->split_path)
		return (-2);
	lst->i = 0;
	lst->path_cmd = ft_strjoin(lst->split_path[lst->i], lst->slash_cmd);
	if (!lst->path_cmd)
		return (-2);
	while (access(lst->path_cmd, F_OK) == -1)
	{
		if (lst->split_path[lst->i] == NULL)
			return (display_error_cmd(lst), -1);
		free(lst->path_cmd);
		lst->path_cmd = ft_strjoin(lst->split_path[lst->i++], lst->slash_cmd);
		if (!lst->path_cmd)
			return (-2);
	}
	//printf("command |%s| ok\n", lst->args[0])
	return (0);
}
