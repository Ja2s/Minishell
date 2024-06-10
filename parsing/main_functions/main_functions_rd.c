/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_functions_rd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:15:55 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/10 17:14:36 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	delimiter_positiver(t_data **data)
{
	int	i;

	i = 0;
	while (i < (*data)->cmd->nb_del)
		command_positiver((*data)->cmd->delimiter[i++]);
}

void	args_positiver(t_data **data)
{
	int	i;

	i = 0;
	while ((*data)->cmd->args[i])
		command_positiver((*data)->cmd->args[i++]);
}

void	redirecter_positiver(t_data **data)
{
	int	i;

	i = 0;
	while ((*data)->cmd->redirecter[i])
		command_positiver((*data)->cmd->redirecter[i++]);
}

int	final_parse(t_data *data)
{
	t_data	*tmp;

	tmp = data;
	if (node_creator(data) == -1)
		return (-1);
	args_positiver(&data);
	if (data->cmd->nb_del > 0)
		delimiter_positiver(&data);
	if (data->cmd->nb_red > 0)
	{
		if (redirecter_finisher(data) == -1)
			return (-1);
		redirecter_positiver(&data);
	}
	if (launch_exec(tmp) == -1)
		return (-1);
	data->exit_code = 0;
	if (data->cmd->nb_del > 0)
		unlink(".heredoc");
	ft_lstclear(&data->cmd);
	free_pipes(data->var.pipes);
	free(data->var.rl);
	if (data->var.mini_env)
	{
		free_pipes(data->var.mini_env);
		data->var.mini_env = NULL;
	}
	free(data->var.rl);
	if (data->var.mini_env)
	{
		free_pipes(data->var.mini_env);
		data->var.mini_env = NULL;
	}
	return (0);
}
