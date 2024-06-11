/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:08:03 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/11 17:57:03 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	handle_sigint_main(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_sig = 1;
}

void	ft_signal(t_data *data, int choice)
{
	if (choice == 1)
	{
		signal(SIGINT, handle_sigint_main);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (choice == 2)
	{
		exit_status(data, 130, "");
		g_sig = 0;
	}
}

void	cleanup(t_data *data)
{
	ft_lstclear(&data->cmd);
	free_pipes(data->var.pipes);
	if (data->var.mini_env)
	{
		free_pipes(data->var.mini_env);
		data->var.mini_env = NULL;
	}
}
