/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gavairon <gavairon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:43:26 by rasamad           #+#    #+#             */
/*   Updated: 2024/05/28 20:54:09 by gavairon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*ft_lstlast_minishell(t_cmd *lst)
{
	t_cmd	*tmp;

	if (!lst)
		return (NULL);
	tmp = lst;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	ft_lstadd_back_minishell(t_cmd **lst, t_cmd *new)
{
	if (*lst)
		ft_lstlast_minishell(*lst)->next = new;
	else
		*lst = new;
}

t_cmd    *ft_lstnew_minishell(void)
{
    t_cmd    *new_elem;

    new_elem = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
    if (!new_elem)
        return (NULL);
    return (new_elem);
}
