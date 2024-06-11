/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:08:03 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/11 16:11:28 by jgavairo         ###   ########.fr       */
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

void	ft_signal(void)
{
	signal(SIGINT, handle_sigint_main);
	signal(SIGQUIT, SIG_IGN);
}

static	void	cleanup(t_data *data)
{
	ft_lstclear(&data->cmd);
	free_pipes(data->var.pipes);
	if (data->var.mini_env)
	{
		free_pipes(data->var.mini_env);
		data->var.mini_env = NULL;
	}
}

int	process_command(t_data *data)
{
	if (data->var.rl[0] != '\0' && syntaxe_error(data, data->var.rl) == 0)
	{
		if (parser(data) == 0)
		{
			if (final_parse(data) == -1)
			{
				cleanup(data);
				return (-1);
			}
		}
	}
	else
		free(data->var.rl);
	return (0);
}

int	run_minishell(t_data *data)
{
	while (1)
	{
		ft_signal();
		if (prompt_customer(data) == 0)
		{
			if (g_sig)
			{
				exit_status(data, 130, "");
				g_sig = 0;		
			}
			else if (process_command(data) == -1)
				return (-1);
		}
		else
			break ;
	}
	return (0);
}
