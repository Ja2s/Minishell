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

int	open_outfile(t_data *data, t_cmd *lst, int *i, int j)
{
	struct stat	statbuf;

	ft_close_outfile(lst);
	j++;
	while (lst->redirecter[*i][j] == ' ')
		j++;
	lst->fd_outfile = open(lst->redirecter[*i] + j, \
	O_CREAT | O_WRONLY | O_TRUNC, 0777);
	stat(lst->redirecter[*i] + j, &statbuf);
	if (S_ISDIR(statbuf.st_mode))
		return (display_is_dir(lst->redirecter[*i] + j), \
		exit_status(data, 1, ""), -1);
	if (lst->fd_outfile == -1)
		return (exit_status(data, 1, ""), \
		display_perror(lst->redirecter[*i] + j), ft_close(lst), -1);
	return (0);
}

int	open_append(t_data *data, t_cmd *lst, int *i, int j)
{
	ft_close_outfile(lst);
	j += 2;
	while (lst->redirecter[*i][j] == ' ')
		j++;
	lst->fd_outfile = open(lst->redirecter[*i] + j, \
	O_CREAT | O_WRONLY | O_APPEND, 0777);
	if (lst->fd_outfile == -1)
		return (exit_status(data, 1, ""), \
		display_perror(lst->redirecter[*i] + j), ft_close(lst), -1);
	return (0);
}

int	open_infile(t_data *data, t_cmd *lst, int *i, int j)
{
	ft_close_infile(lst);
	j++;
	while (lst->redirecter[*i][j] == ' ')
		j++;
	lst->fd_infile = open(lst->redirecter[*i] + j, O_RDONLY, 0777);
	if (lst->fd_infile == -1)
		return (exit_status(data, 1, ""), \
		display_perror(lst->redirecter[*i] + j), ft_close(lst), -1);
	return (0);
}

int	open_redirecter(t_data *data, t_cmd *lst, int *i, int j)
{
	while (lst->redirecter[*i][j])
	{
		if (lst->redirecter[*i][j] == '>' && lst->redirecter[*i][j + 1] == '>')
		{
			if (open_append(data, lst, i, j) == -1)
				return (-1);
			break ;
		}
		else if (lst->redirecter[*i][j] == '>')
		{
			if (open_outfile(data, lst, i, j) == -1)
				return (-1);
			break ;
		}
		else if (lst->redirecter[*i][j] == '<')
		{
			if (open_infile(data, lst, i, j) == -1)
				return (-1);
			break ;
		}
	}
	return (0);
}

int	ft_redirecter(t_data *data, t_cmd *lst)
{
	int	i;
	int	j;

	lst->fd_infile = -1;
	lst->fd_outfile = -1;
	i = 0;
	while (lst->redirecter[i])
	{
		j = 0;
		if (open_redirecter(data, lst, &i, j) == -1)
			return (-1);
		i++;
	}
	return (0);
}
