/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 12:38:44 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/28 11:04:52 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*ft_lstlast(t_cmd *lst)
{
	t_cmd	*last;

	if (!lst)
		return (NULL);
	last = lst;
	while (last->next)
		last = last->next;
	return (last);
}

void	args_free(t_cmd *lst)
{
	int	i;

	i = 0;
	while (i < lst->nb_args + 1)
	{
		free(lst->args[i]);
		i++;
	}
	if (lst->nb_args > 0)
		free(lst->args);
}

void	red_free(t_cmd *lst)
{
	int	i;

	i = 0;
	while (i < lst->nb_red)
	{
		free(lst->redirecter[i]);
		i++;
	}
	if (lst->nb_red > 0)
		free(lst->redirecter);
}

void	del_free(t_cmd *lst)
{
	int	i;

	i = 0;
	while (i < lst->nb_del)
	{
		free(lst->delimiter[i]);
		i++;
	}
	if (lst->nb_del > 0)
		free(lst->delimiter);
}

void	split_path_free(t_cmd *lst)
{
	int	i;

	i = 0;
	if (lst->split_path)
	{
		while (lst->split_path[i])
		{
			free(lst->split_path[i]);
			i++;
		}
		free(lst->split_path);
	}
}
