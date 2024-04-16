/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 18:09:29 by rasamad           #+#    #+#             */
/*   Updated: 2024/04/04 13:17:55 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



void	ft_close(t_lst *elem)
{
	if (elem->redirection && elem->fd_infile != -1)//s'il n'y a pas de redirection les elem->fd ne sont pas initaliser
	{
		close(elem->fd_infile);
		elem->fd_infile = -1;
	}
	if (elem->redirection && elem->fd_outfile != -1)
	{
		close(elem->fd_outfile);
		elem->fd_outfile = -1;
	}
}


void	ft_redirection(t_lst *elem)
{
	int	i;
	int	j;

	elem->fd_infile = -1;
	elem->fd_outfile = -1;	
	i = 0;
	while (elem->redirection[i])
	{
		j = 0;
		while (elem->redirection[i][j])
		{
			if (elem->redirection[i][j] == '>' && elem->redirection[i][j + 1] == '>') // >> 
			{
				j += 2;
				while (elem->redirection[i][j] == ' ')//skip space
					j++;
				//check si je dois close l'ancien en cas de redirection multiple
				elem->fd_outfile = open(elem->redirection[i] + j, O_CREAT | O_WRONLY | O_APPEND, 0777);
				if (elem->fd_outfile == -1)
				{
                    perror("Erreur lors de l'ouverture du fichier de sortie (>>)");
					ft_close(elem);
					elem->open = -1; //permet de verifier le fail dun output alors ne pas exec la cmd
					return;
                }
				break;
			}
			else if (elem->redirection[i][j] == '>')// > 
			{
				j++;
				while (elem->redirection[i][j] == ' ')
					j++;
				//check si je dois close l'ancien en cas de redirection multiple
				elem->fd_outfile = open(elem->redirection[i] + j, O_CREAT | O_WRONLY | O_TRUNC, 0777);
				if (elem->fd_outfile == -1) {
                    perror("Erreur lors de l'ouverture du fichier de sortie (>)");
					ft_close(elem);
					elem->open = -1; //permet de verifier le fail dun output alors ne pas exec la cmd
					return;
                }
				break;
			}
			else if (elem->redirection[i][j] == '<')// <
			{
				j++;
				while (elem->redirection[i][j] == ' ')
					j++;
				elem->fd_infile = open(elem->redirection[i] + j, O_RDONLY, 0777);
				if (elem->fd_infile == -1) {
                    write (2, elem->redirection[i] + j, ft_strlen(elem->redirection[i] + j));
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
