/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 19:20:30 by rasamad           #+#    #+#             */
/*   Updated: 2024/06/11 22:07:33 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_stat_check(int check_access, t_data *data, t_cmd *lst)
{
	struct stat	statbuf;

	if (check_access == -1)
		return (-1);
	if (!lst->args[0])
		return (0);
	if (check_access == 0)
	{
		if (stat(lst->path_cmd, &statbuf) == -1)
			printf("command not Found");
		if (S_ISDIR(statbuf.st_mode))
		{
			exit_status(data, 126, "");
			display_is_dir(lst->args[0]);
			return (-1);
		}
	}
	return (0);
}

void	ft_wait_process(t_data *data, int len_lst)
{
	int	i;
	int	status;

	i = 0;
	ft_close_pipe(data);
	while (i < len_lst)
	{
		status = 0;
		waitpid(-1, &status, 0);
		if (g_sig == 1)
		{
			exit_status(data, 130, "");
			g_sig = 0;
		}
		if (g_sig == 2)
		{
			exit_status(data, 131, "Quit (core dumped)\n");
			g_sig = 0;
		}
		if (WIFEXITED(status) && status != 0)
			exit_status(data, WIFEXITED(status), "");
		i++;
	}
}

int	ft_exec_cmd(t_data *data, t_cmd *lst, int len_lst, int i)
{
	if (ft_builtins_env(lst, data, i) == 0)
	{
		if (ft_is_builtins_no_access(lst) == 0)
			if (ft_stat_check(ft_check_access(data, lst), data, lst) == -1)
				return (free_all_heredoc(data->cmd), ft_close_pipe(data), -1);
		if (i == 1)
		{
			if (ft_first_fork(data, lst) && data->pipe_fd[1] != -1)
				close(data->pipe_fd[1]);
			data->save_pipe = data->pipe_fd[0];
		}
		else if (i < len_lst)
		{
			ft_middle_fork(data, lst);
			close(data->pipe_fd[1]);
			close(data->save_pipe);
			data->save_pipe = data->pipe_fd[0];
		}
		else if (i == len_lst)
			if (ft_last_fork(data, lst))
				close(data->save_pipe);
	}
	return (0);
}

int	ft_check_cmd(t_data *data, t_cmd *lst, int len_lst, int i)
{
	while (lst)
	{
		data->exit_code = 0;
		i++;
		if (lst->redirecter)
			ft_redirecter(data, lst);
		if (lst->next != NULL)
			if (pipe(data->pipe_fd) == -1)
				exit_status(data, 1, "pipe failed\n");
		if (!lst->args[0] && !lst->next)
			return (ft_close_pipe(data), free_all_heredoc(data->cmd), -1);
		if (ft_exec_cmd(data, lst, len_lst, i) == -1)
			return (-1);
		ft_close(lst);
		lst = lst->next;
	}
	return (0);
}

int	launch_exec(t_data *data)
{
	int		i;
	t_cmd	*lst;
	int		len_lst;

	if (data->cmd->args[0] && data->cmd->next == NULL && \
	ft_strcmp(data->cmd->args[0], "exit") == 0)
		return (ft_exit_prog(data));
	lst = data->cmd;
	data->var.mini_env = ft_list_to_tab(data->mini_env);
	if (!data->var.mini_env)
		return (exit_status(data, 1, "malloc error from [list_to_tab]\n"), -1);
	data->save_pipe = 0;
	i = 0;
	len_lst = ft_lstlen(lst);
	if (ft_heredoc(data) == -1)
		return (free_all_heredoc(data->cmd), -1);
	if (ft_check_cmd(data, lst, len_lst, i) == -1)
		return (-1);
	ft_wait_process(data, len_lst);
	free_pipes(data->var.mini_env);
	data->var.mini_env = NULL;
	free_all_heredoc(data->cmd);
	if (data->exit_code != 0)
		return (-1);
	return (0);
}
