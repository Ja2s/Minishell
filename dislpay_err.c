/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dislpay_err.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:19:08 by rasamad           #+#    #+#             */
/*   Updated: 2024/04/01 18:00:15 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_error_cmd(t_lst *var)
{
	ft_putstr_fd("minishell: commande not found: ", 2);
	ft_putstr_fd(var->cmd, 2);
	write(2, "\n", 1);
}

void	display_no_such(t_lst *var)
{
	ft_putstr_fd("minishell: no such file or directory: ", 2);
	ft_putstr_fd(var->cmd, 2);
	write(2, "\n", 1);
}

void	ft_free_access(t_lst *var)
{
	int	i;

	i = 0;
	if (var->split_path != NULL)
	{
		while (var->split_path[i] !=  NULL)
		{
			free(var->split_path[i]);
			var->split_path[i] = NULL;
			i++;
		}
		free(var->split_path);
	}
	if (var->path_cmd != NULL)
	{
		free(var->path_cmd);
		var->path_cmd = NULL;
	}
	if (var->slash_cmd != NULL)
	{
		free(var->slash_cmd);
		var->slash_cmd = NULL;
	}
}

void	ft_free_token(t_lst *var)
{
	int	i;

	i = 0;
	if (var->args)
	{
		while (var->args[i])
		{
			ft_printf("args[%d] = |%s|  ", i, var->args[i]);
			free(var->args[i]);
			var->args[i] = NULL;
			i++;
		}
		free(var->args);
	}
	i = 0;
	ft_printf("\n");
	if (var->redirection)
	{
		while (var->redirection[i])
		{
			ft_printf("redi[%d]  ", i);
			free(var->redirection[i]);
			var->redirection[i] = NULL;
			i++;
		}
		free(var->redirection);
	}
}

void ft_free_lst(t_lst *lst)
{
    t_lst *tmp;

    while (lst)
    {
        tmp = lst;
        lst = lst->next;

        // Libérer la mémoire de l'élément
        free(tmp);
    }
	free(lst);
}
