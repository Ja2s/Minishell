/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:24:55 by rasamad           #+#    #+#             */
/*   Updated: 2024/04/03 14:33:31 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Fonction	: Verifie si la cmd est good
//Renvoi	: -1 --> echec / 0 --> success / -2 ---> malloc crash
int	ft_check_access(t_lst *var, char **envp)
{
	if (ft_strchr(var->cmd, '/'))//verif si path de cmd
	{
		if (access(var->cmd, F_OK) == -1)
			return (display_no_such(var), -1);
		else
		{
			var->path_cmd = var->cmd;
			return (printf("command path |%s| ok\n", var->cmd), 0);
		}
	}
	var->slash_cmd = ft_strjoin("/\0", var->cmd);//ajout du slash au debut de la cmd "/cat"
	if (!var->slash_cmd)
		return (-2);
	var->i = 0;
	if (var->cmd)
	while (ft_strncmp(envp[var->i], "PATH=", 5))
		var->i++;
	var->split_path = ft_split(envp[var->i], ':');
	if (!var->split_path)
		return (-2);
	var->i = 0;
	var->path_cmd = ft_strjoin(var->split_path[var->i], var->slash_cmd);
	if (!var->path_cmd)
		return (-2);
	while (access(var->path_cmd, F_OK) == -1)
	{
		if (var->split_path[var->i] == NULL)
			return (display_error_cmd(var), -1);
		free(var->path_cmd);
		var->path_cmd = ft_strjoin(var->split_path[var->i++], var->slash_cmd);
		if (!var->path_cmd)
			return (-2);
	}
	//printf("command |%s| ok\n", var->cmd)
	return (0);
}
