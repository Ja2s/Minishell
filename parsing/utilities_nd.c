/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities_nd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gavairon <gavairon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 12:46:02 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/25 15:50:46 by gavairon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	path_cmd_free(t_cmd *lst)
{
	if (lst->path_cmd)
		free(lst->path_cmd);
}

void	slash_cmd_free(t_cmd *lst)
{
	if (lst->slash_cmd)
		free(lst->slash_cmd);
}

void	ft_lstdelone(t_cmd *lst)
{
	if (lst)
	{
		
		args_free(lst);
		red_free(lst);
		del_free(lst);
		split_path_free(lst);
		path_cmd_free(lst);
		slash_cmd_free(lst);
		free(lst);
	}
}

void	ft_lstclear(t_cmd **lst)
{
	t_cmd	*tmp;

	while (*lst)
	{
		tmp = (*lst)->next;
		ft_lstdelone(*lst);
		*lst = tmp;
	}
}

int	ft_lstlen(t_cmd *elem)
{
	int	i;

	i = 0;
	while (elem)
	{
		i++;
		elem = elem->next;
	}
	return (i);
}
