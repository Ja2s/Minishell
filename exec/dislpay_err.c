/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dislpay_err.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:19:08 by rasamad           #+#    #+#             */
/*   Updated: 2024/06/07 12:47:51 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	display_error_cmd(t_cmd *lst)
{
	ft_putstr_fd(lst->args[0], 2);
	ft_putstr_fd(": command not found", 2);
	write(2, "\n", 1);
}

void	display_perror(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	perror(" ");
}

void	display_no_such(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": No such file or directory", 2);
	write(2, "\n", 1);
}

void	ft_free_access(t_cmd *lst)
{
	int	i;

	i = 0;
	if (lst->split_path != NULL)
	{
		while (lst->split_path[i] !=  NULL)
		{
			free(lst->split_path[i]);
			lst->split_path[i] = NULL;
			i++;
		}
		free(lst->split_path);
	}
	if (lst->path_cmd != NULL)
	{
		free(lst->path_cmd);
		lst->path_cmd = NULL;
	}
	if (lst->slash_cmd != NULL)
	{
		free(lst->slash_cmd);
		lst->slash_cmd = NULL;
	}
}

void	ft_free_token(t_cmd *lst)
{
	int	i;

	i = 0;
	if (lst->args)
	{
		while (lst->args[i])
		{
			printf("args[%d] = |%s|  ", i, lst->args[i]);
			free(lst->args[i]);
			lst->args[i] = NULL;
			i++;
		}
		free(lst->args);
	}
	i = 0;
	printf("\n");
	if (lst->redirecter)
	{
		while (lst->redirecter[i])
		{
			printf("redi[%d]  ", i);
			free(lst->redirecter[i]);
			lst->redirecter[i] = NULL;
			i++;
		}
		free(lst->redirecter);
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
