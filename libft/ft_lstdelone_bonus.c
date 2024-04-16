/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 14:42:42 by jgavairo          #+#    #+#             */
/*   Updated: 2024/04/10 14:56:59 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	args_free(t_cmd *lst)
{
	int	i;

	i = 0;
	while (i < lst->nb_args)
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

void	ft_lstdelone(t_cmd *lst)
{
	args_free(lst);
	red_free(lst);
	del_free(lst);
	if (lst)
		free(lst);
}
