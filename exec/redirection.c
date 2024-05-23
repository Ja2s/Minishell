/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirecter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 18:09:29 by rasamad           #+#    #+#             */
/*   Updated: 2024/04/16 17:02:39 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	 ft_close(t_cmd *lst)
{
	if (lst->redirecter && lst->fd_infile != -1)//s'il n'y a pas de redirecter les lst->fd ne sont pas initaliser
	{
		close(lst->fd_infile);
		lst->fd_infile = -1;
	}
	if (lst->redirecter && lst->fd_outfile != -1)
	{
		close(lst->fd_outfile);
		lst->fd_outfile = -1;
	}
}

int	ft_redirecter(t_data *data)
{
	int	i;
	int	j;
	t_cmd	*lst;

	lst = data->cmd;
	lst->fd_infile = -1;
	lst->fd_outfile = -1;	
	i = 0;
	while (lst->redirecter[i])
	{
		j = 0;
		while (lst->redirecter[i][j])
		{
			if (lst->redirecter[i][j] == '>' && lst->redirecter[i][j + 1] == '>') // >> 
			{
				j += 2;
				while (lst->redirecter[i][j] == ' ')//skip space
					j++;
				//check si je dois close l'ancien en cas de redirecter multiple
				lst->fd_outfile = open(lst->redirecter[i] + j, O_CREAT | O_WRONLY | O_APPEND, 0777);
				if (lst->fd_outfile == -1)
					return(exit_status(data, 1, " :Open failed (>>)\n"), ft_close(lst), -1);
				break;
			}
			else if (lst->redirecter[i][j] == '>')// > 
			{
				j++;
				while (lst->redirecter[i][j] == ' ')
					j++;
				//check si je dois close l'ancien en cas de redirecter multiple
				lst->fd_outfile = open(lst->redirecter[i] + j, O_CREAT | O_WRONLY | O_TRUNC, 0777);
				if (lst->fd_outfile == -1)
					return(exit_status(data, 1, " :Open failed (>)\n"), ft_close(lst), -1);
				break;
			}
			else if (lst->redirecter[i][j] == '<')// <
			{
				j++;
				while (lst->redirecter[i][j] == ' ')
					j++;
				lst->fd_infile = open(lst->redirecter[i] + j, O_RDONLY, 0777);
				if (lst->fd_infile == -1) {
                    write (2, lst->redirecter[i] + j, ft_strlen(lst->redirecter[i] + j));
                    perror(" (<)");
					return(exit_status(data, 1, ""), ft_close(lst), -1);
                }
				break;
			}
		}
		i++;
	}
	return (0);
}
