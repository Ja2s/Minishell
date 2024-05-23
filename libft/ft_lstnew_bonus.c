/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gavairon <gavairon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:26:35 by jgavairo          #+#    #+#             */
/*   Updated: 2024/04/09 13:19:03 by gavairon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_cmd	*ft_lstnew(void)
{
	t_cmd	*new_elem;

	new_elem = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_elem)
		return (NULL);
	new_elem->nb_args = 0;
	new_elem->nb_del = 0;
	new_elem->nb_flags = 0;
	new_elem->nb_red = 0;
	new_elem->delimiter = NULL;
	new_elem->redirecter = NULL;
	new_elem->args = NULL;
	new_elem->next = NULL;
	new_elem->start = NULL;
	new_elem->heredoc = false;
	return (new_elem);
}
