/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 19:02:59 by rasamad           #+#    #+#             */
/*   Updated: 2024/06/11 21:50:52 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_builtins_env_fork(t_data *data, t_cmd *lst)
{
	if (ft_strcmp(lst->args[0], "export") == 0)
	{
		ft_export(data, &data->mini_env, lst);
		return (1);
	}
	else if (ft_strcmp(lst->args[0], "unset") == 0)
	{
		ft_unset(&data);
		return (1);
	}
	else if (ft_strcmp(lst->args[0], "env") == 0)
	{
		env_cmd(data->mini_env);
		return (1);
	}
	else if (ft_strcmp(lst->args[0], "cd") == 0)
	{
		ft_cd(data);
		return (1);
	}
	return (0);
}

void	ft_last_execve(t_data *data, t_cmd *lst)
{
	if (lst->redirecter && lst->fd_outfile > 0)
		if (dup2(lst->fd_outfile, STDOUT_FILENO) == -1)
			ft_error_exit(data, lst);
	ft_close(lst);
	ft_close_pipe(data);
	ft_no_execve(data, lst);
	execve(lst->path_cmd, lst->args, data->var.mini_env);
	perror("execve 3 : failed ");
	ft_free_all_fork(data);
	exit(EXIT_FAILURE);
}

void	ft_last_children(t_data *data, t_cmd *lst)
{
	if (lst->redirecter && lst->fd_infile > 0 && lst->end_heredoc == 0)
	{
		if (dup2(lst->fd_infile, STDIN_FILENO) == -1)
			ft_error_exit(data, lst);
	}
	else if (lst->heredoc_content && lst->end_heredoc == 1)
	{
		if (ft_fd_heredoc(lst) == -1)
			ft_error_exit(data, lst);
		lst->fd_str_rand = open(".heredoc", O_RDONLY);
		if (lst->fd_str_rand == -1 || \
		dup2(lst->fd_str_rand, STDIN_FILENO) == -1)
			ft_error_exit(data, lst);
		close(lst->fd_str_rand);
	}
	else if (data->save_pipe)
	{
		if (dup2(data->save_pipe, STDIN_FILENO) == -1)
			ft_error_exit(data, lst);
	}
	ft_last_execve(data, lst);
}

pid_t	ft_last_fork(t_data *data, t_cmd *lst)
{
	pid_t	pid;

	signal(SIGINT, handle_sigint_fork);
	signal(SIGQUIT, handle_sigquit_fork);
	pid = fork();
	if (pid < 0)
		return (perror("fork last failed :"), -1);
	if (pid == 0)
		ft_last_children(data, lst);
	return (pid);
}
