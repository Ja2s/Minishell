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

#include "minishell.h"
#include "libft/libft.h"



void	ft_close(t_cmd *elem)
{
	if (elem->redirecter && elem->fd_infile != -1)//s'il n'y a pas de redirecter les elem->fd ne sont pas initaliser
	{
		close(elem->fd_infile);
		elem->fd_infile = -1;
	}
	if (elem->redirecter && elem->fd_outfile != -1)
	{
		close(elem->fd_outfile);
		elem->fd_outfile = -1;
	}
}


void	ft_redirecter(t_cmd *elem)
{
	int	i;
	int	j;

	elem->fd_infile = -1;
	elem->fd_outfile = -1;	
	i = 0;
	while (elem->redirecter[i])
	{
		j = 0;
		while (elem->redirecter[i][j])
		{
			if (elem->redirecter[i][j] == '>' && elem->redirecter[i][j + 1] == '>') // >> 
			{
				j += 2;
				while (elem->redirecter[i][j] == ' ')//skip space
					j++;
				//check si je dois close l'ancien en cas de redirecter multiple
				elem->fd_outfile = open(elem->redirecter[i] + j, O_CREAT | O_WRONLY | O_APPEND, 0777);
				if (elem->fd_outfile == -1)
				{
                    perror("Erreur lors de l'ouverture du fichier de sortie (>>)");
					ft_close(elem);
					elem->open = -1; //permet de verifier le fail dun output alors ne pas exec la cmd
					return;
                }
				break;
			}
			else if (elem->redirecter[i][j] == '>')// > 
			{
				j++;
				while (elem->redirecter[i][j] == ' ')
					j++;
				//check si je dois close l'ancien en cas de redirecter multiple
				elem->fd_outfile = open(elem->redirecter[i] + j, O_CREAT | O_WRONLY | O_TRUNC, 0777);
				if (elem->fd_outfile == -1) {
                    perror("Erreur lors de l'ouverture du fichier de sortie (>)");
					ft_close(elem);
					elem->open = -1; //permet de verifier le fail dun output alors ne pas exec la cmd
					return;
                }
				break;
			}
			else if (elem->redirecter[i][j] == '<')// <
			{
				j++;
				while (elem->redirecter[i][j] == ' ')
					j++;
				elem->fd_infile = open(elem->redirecter[i] + j, O_RDONLY, 0777);
				if (elem->fd_infile == -1) {
                    write (2, elem->redirecter[i] + j, ft_strlen(elem->redirecter[i] + j));
                    perror(" (<)");
					ft_close(elem);
					elem->open = -1; //permet de verifier le fail dun input alors ne pas exec la cmd
					return;
                }
				break;
			}
		}
		i++;
	}
}
