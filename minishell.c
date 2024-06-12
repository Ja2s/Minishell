/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 11:02:31 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/12 14:14:15 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

volatile sig_atomic_t	g_sig = 0;

int	ft_check_num(t_data *data)
{
	int	i;

	if (data->cmd->args[1][0] != '+' && data->cmd->args[1][0] != '-' && \
	(!(data->cmd->args[1][0] >= '0' && data->cmd->args[1][0] <= '9')))
		return (-1);
	if (data->cmd->args[1][0] == '+' \
	&& (!(data->cmd->args[1][1] >= '0' && data->cmd->args[1][1] <= '9')))
		return (-1);
	else if (data->cmd->args[1][0] == '-' && \
	(!(data->cmd->args[1][1] >= '0' && data->cmd->args[1][1] <= '9')))
		return (-1);
	i = 1;
	while (data->cmd->args[1][i])
	{
		if ((!(data->cmd->args[1][i] >= '0' && data->cmd->args[1][i] <= '9')))
			return (-1);
		i++;
	}
	return (0);
}

void	clear_exit(t_data *data)
{
	ft_lstclear(&data->cmd);
	free_pipes(data->var.pipes);
	free_pipes(data->var.mini_env);
	free_env(data->mini_env);
	rl_clear_history();
}

int	ft_exit_prog(t_data *data)
{
	int	exit_stat;

	exit_stat = 0;
	printf("exit\n");
	if (data->cmd->args[1] && ft_check_num(data) == -1)
	{
		printf("minishell: exit: %s: numeric argument required\n", \
		data->cmd->args[1]);
		ft_lstclear(&data->cmd);
		clear_exit(data);
		exit(2);
	}
	if (data->cmd->nb_args > 2)
		return (exit_status(data, 1, \
		"minishell: exit: too many arguments\n"), -1);
	if (data->cmd->args[1])
		exit_stat = ft_atoi(data->cmd->args[1]) % 256;
	clear_exit(data);
	exit(exit_stat);
	return (0);
}

void	ft_close_pipe(t_data *data)
{
	if (data->pipe_fd[0] != -1)
	{
		close(data->pipe_fd[0]);
		data->pipe_fd[0] = -1;
	}
	if (data->pipe_fd[1] != -1)
	{
		close(data->pipe_fd[1]);
		data->pipe_fd[1] = -1;
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		i;
	t_data	data;

	(void)argc;
	(void)argv;
	i = 0;
	data.exit_code = 0;
	if (minishell_starter(envp, &data) == -1)
		return (printf("malloc error from [main]\n"), -1);
	while (1)
	{
		ft_signal();
		if (prompt_customer(&data) == 0)
		{
			if (g_sig)
			{
				exit_status(&data, 130, "");
				g_sig = 0;		
			}
			else if (data.var.rl[0] != '\0' && syntaxe_error(&data, data.var.rl) == 0)
			{
				if (parser(&data) == 0)
				{
					if (final_parse(&data) == -1)
					{
						ft_lstclear(&data.cmd);
						free_pipes(data.var.pipes);
						if (data.var.mini_env)
						{
							free_pipes(data.var.mini_env);
							data.var.mini_env = NULL;
						}
					}
				}
			}
			else
				free(data.var.rl);
		}
		else
			//free()
			break ;
	}
	free_env(data.mini_env);
	rl_clear_history();
}
