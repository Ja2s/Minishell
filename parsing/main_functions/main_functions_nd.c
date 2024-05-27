/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_functions_nd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 12:22:23 by gavairon          #+#    #+#             */
/*   Updated: 2024/05/27 17:38:08 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	command_positiver(char *pipes)
{
	size_t	i;

	i = 0;
	while (pipes[i])
	{
		if (pipes[i] < 0)
			pipes[i] = pipes[i] * -1;
		i++;
	}
}

char	*copy_w_cote(char *src, char *dest)
{
	int	i;
	int	p;

	p = 0;
	i = 0;
	dest = malloc(sizeof (char) * (ft_strlen(src) + 1));
	if (!dest)
		return (NULL);
	while (src[i])
	{
		if (src[i] == 34 || src[i] == 39)
			i++;
		else
			dest[p++] = src[i++];
	}
	dest[p] = '\0';
	return (free(src), dest);
}

char	**input_copyer(char **input, char **input_copy)
{
	int	i;

	i = 0;
	while (input[i])
		i++;
	input_copy = ft_calloc(i + 1, sizeof (char *));
	if (!input_copy)
		return (NULL);
	i = 0;
	while (input[i])
	{
		input_copy[i] = copy_w_cote(input[i], input_copy[i]);
		i++;
	}
	return (input_copy);
}

void	data_initializer(t_data *data)
{
	(*data).var.input = NULL;
	(*data).var.rl = NULL;
	(*data).mini_env = NULL;
	(*data).var.mini_env = NULL;
	(*data).var.input_copy = NULL;
	(*data).var.input = NULL;
	(*data).cmd = NULL;
	data->pipe_fd[0] = -1;
	data->pipe_fd[1] = -1;
	data->ambigous = 0;
}

int	redirecter_finisher(t_data *data)
{
	int		i;

	i = 0;
	data->cmd->red_copy = \
	input_copyer(data->cmd->redirecter, data->cmd->red_copy);
	data->cmd->redirecter = data->cmd->red_copy;
	data->cmd->red_copy = NULL;
	return (0);
}
