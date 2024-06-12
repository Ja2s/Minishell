/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 13:56:31 by rasamad           #+#    #+#             */
/*   Updated: 2024/06/12 14:06:16 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_close_infile(t_cmd *lst)
{
	if (lst->redirecter && lst->fd_infile != -1)
	{
		close(lst->fd_infile);
		lst->fd_infile = -1;
	}
}

void	ft_close_outfile(t_cmd *lst)
{
	if (lst->redirecter && lst->fd_outfile != -1)
	{
		close(lst->fd_outfile);
		lst->fd_outfile = -1;
	}
}

void	ft_close(t_cmd *lst)
{
	ft_close_infile(lst);
	ft_close_outfile(lst);
}
