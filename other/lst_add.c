/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gavairon <gavairon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:43:26 by rasamad           #+#    #+#             */
/*   Updated: 2024/05/08 16:23:01 by gavairon         ###   ########.fr       */
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

void	int_initalizer(t_cmd **new_elem)
{
	(*new_elem)->i = 0;
	(*new_elem)->open = 0;
	(*new_elem)->nb_red = 0;
	(*new_elem)->nb_del = 0;
	(*new_elem)->nb_args = 0;
	(*new_elem)->nb_flags = 0;
	(*new_elem)->fd_infile = 0;
	(*new_elem)->fd_outfile = 0;
	(*new_elem)->end_heredoc = 0;
	(*new_elem)->expand_heredoc = 0;
}

t_cmd	*ft_lstnew_minishell(void)
{
	t_cmd	*new_elem;

	new_elem = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_elem)
		return (NULL);
	int_initalizer(&new_elem);
	new_elem->heredoc = false;
	new_elem->args = NULL;
	new_elem->next = NULL;
	new_elem->start = NULL;
	new_elem->path_cmd = NULL;
	new_elem->delimiter = NULL;
	new_elem->slash_cmd = NULL;
	new_elem->redirecter = NULL;
	new_elem->split_path = NULL;
	new_elem->heredoc_content = NULL;
	return (new_elem);
}
