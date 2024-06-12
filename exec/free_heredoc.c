/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:39:42 by rasamad           #+#    #+#             */
/*   Updated: 2024/06/12 13:07:55 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_all_heredoc(t_cmd *lst)
{
	while (lst)
	{
		if (lst->heredoc_content)
		{
			free(lst->heredoc_content);
			lst->heredoc_content = NULL;
		}
		lst = lst->next;
	}
}

void	ft_free_heredoc(t_cmd *lst)
{
	int	i;

	i = 0;
	if (lst->heredoc_content)
	{
		free(lst->heredoc_content);
		lst->heredoc_content = NULL;
	}
}

void	del_first(char *line, t_cmd *lst)
{
	free(line);
	lst->heredoc_content = malloc(1 * sizeof(char));
	lst->heredoc_content[0] = '\n';
	lst->del_one = 1;
}
