/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:35:20 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/11 11:32:06 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_isspace(char c)
{
	if (c == '\f' || c == '\t' || c == '\n' || \
	c == '\r' || c == '\v' || c == ' ')
		return (1);
	return (0);
}

void	printf_title(void)
{
	printf("\033[93m");
	printf("		           _       _     _          _ _ \n");
	usleep(150000);
	printf("\033[33m");
	printf("		          (_)     (_)   | |        | | |\n");
	usleep(150000);
	printf("\033[33;2m");
	printf("		 _ __ ___  _ _ __  _ ___| |__   ___| | |\n");
	usleep(150000);
	printf("\033[38;5;220m");
	printf("		| '_ ` _ \\| | '_ \\| / __| '_ \\ / _ \\ | |\n");
	usleep(150000);
	printf("\033[38;5;228m");
	printf("		| | | | | | | | | | \\__ \\ | | |  __/ | |\n");
	usleep(150000);
	printf("\033[38;5;229m");
	printf("		|_| |_| |_|_|_| |_|_|___/_| |_|\\___|_|_|\n");
	usleep(150000);
	printf("\033[38;5;230m");
	printf("		                                        \n");
	usleep(150000);
	printf("\033[0m");
}

void	free_pipes(char **pipes)
{
	int	i;

	i = 0;
	if (pipes)
	{
		while (pipes[i])
		{
			free(pipes[i]);
			i++;
		}
		free(pipes);
	}
}

void	exit_status(t_data *data, int code, char *message)
{
	data->exit_code = code;
	write(2, message, ft_strlen(message));
}

void	exit_status_n_free(t_data *data, int code, char *message)
{
	data->exit_code = code;
	write(2, message, ft_strlen(message));
}
