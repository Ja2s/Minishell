/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 18:15:31 by rasamad           #+#    #+#             */
/*   Updated: 2024/06/11 21:48:26 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_error_exit(t_data *data, t_cmd *lst)
{
	perror("");
	ft_close_pipe(data);
	ft_close(lst);
	exit(EXIT_FAILURE);
}

void	ft_no_execve(t_data *data, t_cmd *lst)
{
	if (!lst->args[0] || data->exit_code != 0 || \
	ft_strcmp(lst->args[0], "exit") == 0 || \
	ft_builtins(lst) != 0 || ft_builtins_env_fork(data, lst) != 0)
	{
		ft_free_all_fork(data);
		exit(0);
	}
}

void	ft_first_execve(t_data *data, t_cmd *lst)
{
	if (lst->redirecter && lst->fd_outfile > 0)
	{
		if (dup2(lst->fd_outfile, STDOUT_FILENO) == -1)
			ft_error_exit(data, lst);
	}
	else if (lst->next)
	{
		if (dup2(data->pipe_fd[1], STDOUT_FILENO) == -1)
			ft_error_exit(data, lst);
		ft_close_pipe(data);
	}
	ft_close(lst);
	ft_close_pipe(data);
	ft_no_execve(data, lst);
	execve(lst->path_cmd, lst->args, data->var.mini_env);
	perror("execve 1 : failed ");
	ft_free_all_fork(data);
	exit(EXIT_FAILURE);
}

void	ft_first_children(t_data *data, t_cmd *lst)
{
	if (lst->redirecter && lst->fd_infile > 0 && lst->end_heredoc == 0)
	{
		if (dup2(lst->fd_infile, STDIN_FILENO) == -1)
			ft_error_exit(data, lst);
	}
	else if (lst->nb_del > 0 && lst->end_heredoc == 1)
	{
		if (ft_fd_heredoc(lst) == -1)
			ft_error_exit(data, lst);
		lst->fd_str_rand = open(".heredoc", O_RDONLY);
		if (lst->fd_str_rand == -1 || \
			dup2(lst->fd_str_rand, STDIN_FILENO) == -1)
			ft_error_exit(data, lst);
		close(lst->fd_str_rand);
	}
	ft_first_execve(data, lst);
}

pid_t	ft_first_fork(t_data *data, t_cmd *lst)
{
	pid_t	pid;

	signal(SIGINT, handle_sigint_fork);
	signal(SIGQUIT, handle_sigquit_fork);
	pid = fork();
	if (pid < 0)
		return (perror("fork first failed :"), -1);
	if (pid == 0)
		ft_first_children(data, lst);
	return (pid);
}
