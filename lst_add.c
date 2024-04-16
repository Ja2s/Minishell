/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:43:26 by rasamad           #+#    #+#             */
/*   Updated: 2024/03/25 17:28:27 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lst	*ft_lstlast_minishell(t_lst *lst)
{
	t_lst	*tmp;

	if (!lst)
		return (NULL);
	tmp = lst;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	ft_lstadd_back_minishell(t_lst **lst, t_lst *new)
{
	if (*lst)
		ft_lstlast_minishell(*lst)->next = new;
	else
		*lst = new;
}

t_lst	*ft_lstnew_minishell(void)
{
	t_lst	*new;

	new = malloc(sizeof(t_lst));
	if (!new)
		return (NULL);
	new->next = NULL;
	return (new);
}