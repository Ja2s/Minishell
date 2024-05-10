/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:35:20 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/10 16:25:25 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_printf_struct(t_cmd *cmd)
{
	int	i;
	
	i = 0;
	while (cmd)
	{
		printf("\n\n\n\033[33m--------------=Command name=--------------\033[0m\n");
		printf("	Commande -> |%s|\n", cmd->args[i]);
		i++;
		printf("\033[33m----------------=Arguments=---------------\033[0m\n");
		while (i < cmd->nb_args)
			{
				printf("	Arg[%d] -> |%s|\n", i, cmd->args[i]);
				i++;
			}
		i = 0;
		if (cmd->nb_red > 0)
		{
			printf("\033[33m-------------=Redirections=-------------\033[0m\n");
			while(i < cmd->nb_red)
			{
				printf("		Redirecter -> |%s|\n", cmd->redirecter[i]);
				i++;
			}
		}
		i = 0;
		if (cmd->heredoc == true)
		{
			printf("\033[35m-------------=Heredoc (true)=-------------\033[0m\n");
			while (i < cmd->nb_del)
			{
				printf("		  delimiter -> |%s|\n", cmd->delimiter[i]);
				i++;
			}
		}
		i = 0;
		

		printf("\033[35m-------------=End of command=-------------\033[0m\n\n\n");
		if (cmd->next)
			cmd = cmd->next;
		else
			return (-1);
	}
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
