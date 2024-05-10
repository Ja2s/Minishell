/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gavairon <gavairon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:27:51 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/08 14:01:28 by gavairon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	redirect_counter(char *pipes)
{
	int	i;
	int	len;

	len = 0;
	i = 0;
	while (pipes[i])
	{
		if (pipes[i] == '<' || pipes[i] == '>')
		{
			if (pipes[i] == '<' && pipes[i + 1] == '<')
				i = i + 2;
			else
			{
				len++;
				i++;
				if (pipes[i] == '<' || pipes[i] == '>')
					i++;
			}
		}
		else
			i++;
	}
	return (len);
}

int	redirecter(char *pipes, t_cmd **cmd)
{
	int	i;
	int	len;
	int	start;
	int	x;

	i = 0;
	len = 0;
	x = 0;
	len = redirect_counter(pipes);
	(*cmd)->nb_red = len;
	if (len > 0)
	{
		(*cmd)->redirecter = malloc(sizeof (char *) * (len + 1));
		if (!(*cmd)->redirecter)
			return (-1);
		len = 0;
		while (pipes[i])
		{
			if (pipes[i] == '<' || pipes[i] == '>')
			{
				if (pipes[i] == '<' && pipes[i + 1] == '<')
				{
					i += 2;
					while (pipes[i] && pipes[i] == ' ')
						i++;
					while (pipes[i] && pipes[i] != ' ' && pipes[i] != '<' && pipes[i] != '>')
						i++;
				}
				else
				{
					start = i;
					i++;
					len++;
					if (pipes[i] == '<' || pipes[i] == '>')
					{
						i++;
						len++;
					}
					while (pipes[i] && pipes[i] == ' ')
					{
						i++;
						len++;
					}
					while (pipes[i] && pipes[i] != ' ' && pipes[i] != '<' && pipes[i] != '>')
					{
						i++;
						len++;
					}
					(*cmd)->redirecter[x] = ft_substr(pipes, start, len);
					x++;
					len = 0;
				}
			}
			else
				i++;
		}
		(*cmd)->redirecter[x] = NULL;
	}
	return (0);
}

int	len_calculator(char	*pipes)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	while (pipes[i])
	{
		while ((pipes[i]) && (pipes[i] != '<' && pipes[i] != '>'))
		{
			x++;
			i++;
		}
		if ((pipes[i]) && (pipes[i] == '<' || pipes[i] == '>'))
		{
			i++;
			if (pipes[i] == '<' || pipes[i] == '>')
				i++;
			while (pipes[i] && pipes[i] == ' ')
				i++;
			while (pipes[i] && pipes[i] != ' ' && pipes[i] != '<' && pipes[i] != '>')
				i++;
		}
	}
	return (x);
}

char	*redirect_deleter(char	*pipes)
{
	int		i;
	int		len;
	char	*tmp;

	tmp = NULL;
	i = 0;
	len = 0;
	len = len_calculator(pipes);
	tmp = ft_calloc((len + 1), sizeof(char));
	if (!tmp)
		return (NULL);
	len = 0;
	while (pipes[i])
	{
		while (pipes[i] && pipes[i] != '<' && pipes[i] != '>')
		{
			if (pipes[i] == ' ' && tmp[len] == ' ')
				i++;
			else
			{
				tmp[len] = pipes[i];
				len++;
				i++;
			}
		}
		if ((pipes[i]) && (pipes[i] == '<' || pipes[i] == '>'))
		{
			i++;
			if (pipes[i] == '<' || pipes[i] == '>')
				i++;
			while (pipes[i] && pipes[i] == ' ')
				i++;
			while (pipes[i] && pipes[i] != ' ' && pipes[i] != '<' && pipes[i] != '>')
				i++;
		}
	}
	free(pipes);
	return (tmp);
}
