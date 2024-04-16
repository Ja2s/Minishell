/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:24:55 by rasamad           #+#    #+#             */
/*   Updated: 2024/04/16 17:30:27 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

//Fonction	: Verifie si la args[0] est good
//Renvoi	: -1 --> echec / 0 --> success / -2 ---> malloc crash
int	ft_check_access(t_cmd *var, char **envp)
{
	if (ft_strchr(var->args[0], '/'))//verif si path de args[0]
	{
		if (access(var->args[0], F_OK) == -1)
			return (display_no_such(var), -1);
		else
		{
			var->path_cmd = var->args[0];
			return (printf("command path |%s| ok\n", var->args[0]), 0);
		}
	}
	var->slash_cmd = ft_strjoin("/\0", var->args[0]);//ajout du slash au debut de la args[0] "/cat"
	if (!var->slash_cmd)
		return (-2);
	var->i = 0;
	if (var->args[0])
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
	//printf("command |%s| ok\n", var->args[0])
	return (0);
}
