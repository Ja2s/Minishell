/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:27:51 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/24 11:32:23 by jgavairo         ###   ########.fr       */
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

static int	skip_spaces(char *pipes, int i)
{
	while (pipes[i] && pipes[i] == ' ')
		i++;
	return (i);
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
					if ((*cmd)->redirecter[x] == NULL)
						return (-1);
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
			while (pipes[i] && pipes[i] != ' ' && \
			pipes[i] != '<' && pipes[i] != '>')
				i++;
		}
	}
	return (x);
}

static int	skip_redirections(char *pipes, int i)
{
	if ((pipes[i] == '<' || pipes[i] == '>'))
	{
		i++;
		if (pipes[i] == '<' || pipes[i] == '>')
			i++;
		while (pipes[i] && ft_isspace(pipes[i]) == 1)
			i++;
		while (pipes[i] && ft_isspace(pipes[i]) == 0 && \
		pipes[i] != '<' && pipes[i] != '>')
			i++;
	}
	return (i);
}

static void	copy_non_redirect_chars(char *pipes, char *tmp, int *i, int *len)
{
	while (pipes[*i] && pipes[*i] != '<' && pipes[*i] != '>')
	{
		if (ft_isspace(pipes[*i]) == 1 && ft_isspace(tmp[*len]) == 1)
			(*i)++;
		else
		{
			tmp[*len] = pipes[*i];
			(*len)++;
			(*i)++;
		}
	}
}

char	*redirect_deleter(char *pipes)
{
	int		i;
	int		len;
	char	*tmp;

	i = 0;
	len = 0;
	len = len_calculator(pipes);
	tmp = ft_calloc((len + 1), sizeof(char));
	if (!tmp)
		return (NULL);
	len = 0;
	while (pipes[i])
	{
		copy_non_redirect_chars(pipes, tmp, &i, &len);
		i = skip_redirections(pipes, i);
	}
	free(pipes);
	return (tmp);
}
