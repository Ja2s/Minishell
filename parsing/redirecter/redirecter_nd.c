/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirecter_nd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:08:17 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/10 14:24:10 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	redirecter_helper_nd(char *pipes, t_cmd **cmd, t_int *var)
{
	(*var).start = (*var).i;
	(*var).i++;
	(*var).len++;
	if (pipes[(*var).i] == '<' || pipes[(*var).i] == '>')
	{
		(*var).i++;
		(*var).len++;
	}
	while (pipes[(*var).i] && ft_isspace(pipes[(*var).i]) == 1)
	{
		(*var).i++;
		(*var).len++;
	}
	while (pipes[(*var).i] && ft_isspace(pipes[(*var).i]) == 0 && \
	pipes[(*var).i] != '<' && pipes[(*var).i] != '>')
	{
		(*var).i++;
		(*var).len++;
	}
	(*cmd)->redirecter[(*var).x] = ft_substr(pipes, (*var).start, (*var).len);
	if ((*cmd)->redirecter[(*var).x] == NULL)
		return (-1);
	(*var).x++;
	(*var).len = 0;
	return (0);
}

int	redirecter_helper(char *pipes, t_cmd **cmd, t_int *var)
{
	if (pipes[(*var).i] == '<' && pipes[(*var).i + 1] == '<')
	{
		(*var).i += 2;
		while (pipes[(*var).i] && ft_isspace(pipes[(*var).i]) == 1)
			(*var).i++;
		while (pipes[(*var).i] && ft_isspace(pipes[(*var).i]) == 0 && \
		pipes[(*var).i] != '<' && pipes[(*var).i] != '>')
			(*var).i++;
	}
	else
		if (redirecter_helper_nd(pipes, cmd, var) == -1)
			return (-1);
	return (0);
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
