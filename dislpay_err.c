/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dislpay_err.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:19:08 by rasamad           #+#    #+#             */
/*   Updated: 2024/04/16 17:30:21 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

void	display_error_cmd(t_cmd *var)
{
	ft_putstr_fd("minishell: commande not found: ", 2);
	ft_putstr_fd(var->args[0], 2);
	write(2, "\n", 1);
}

void	display_no_such(t_cmd *var)
{
	ft_putstr_fd("minishell: no such file or directory: ", 2);
	ft_putstr_fd(var->args[0], 2);
	write(2, "\n", 1);
}

void	ft_free_access(t_cmd *var)
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

void	ft_free_token(t_cmd *var)
{
	int	i;

	i = 0;
	if (var->args)
	{
		while (var->args[i])
		{
			printf("args[%d] = |%s|  ", i, var->args[i]);
			free(var->args[i]);
			var->args[i] = NULL;
			i++;
		}
		free(var->args);
	}
	i = 0;
	printf("\n");
	if (var->redirecter)
	{
		while (var->redirecter[i])
		{
			printf("redi[%d]  ", i);
			free(var->redirecter[i]);
			var->redirecter[i] = NULL;
			i++;
		}
		free(var->redirecter);
	}
}

void ft_free_lst(t_cmd *lst)
{
    t_cmd *tmp;

    while (lst)
    {
        tmp = lst;
        lst = lst->next;

        // Libérer la mémoire de l'élément
        free(tmp);
    }
	free(lst);
}
