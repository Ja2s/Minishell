/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   middle_fork.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 18:57:36 by rasamad           #+#    #+#             */
/*   Updated: 2024/06/11 21:47:33 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_fd_heredoc(t_cmd *lst)
{
	int	len_heredoc;

	len_heredoc = ft_strlen(lst->heredoc_content);
	lst->fd_str_rand = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (lst->fd_str_rand == -1)
	{
		perror("Erreur lors de la création du fichier");
		return (-1);
	}
	if (lst->del_one == 1)
		return (write(lst->fd_str_rand, "\0", 1), 0);
	write(lst->fd_str_rand, lst->heredoc_content, len_heredoc);
	close(lst->fd_str_rand);
	return (0);
}

void	ft_free_all_fork(t_data *data)
{
	free_all_heredoc(data->cmd);
	rl_clear_history();
	ft_lstclear(&data->cmd);
	free_env(data->mini_env);
	free_pipes(data->var.mini_env);
	free_pipes(data->var.pipes);
}

void	ft_middle_execve(t_data *data, t_cmd *lst)
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
		close(data->pipe_fd[1]);
	}
	ft_close(lst);
	ft_no_execve(data, lst);
	execve(lst->path_cmd, lst->args, data->var.mini_env);
	perror("execve 2 failed : ");
	ft_free_all_fork(data);
	exit(EXIT_FAILURE);
}

void	ft_middle_children(t_data *data, t_cmd *lst)
{
	close(data->pipe_fd[0]);
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
		if (dup2(lst->fd_str_rand, STDIN_FILENO) == -1)
			ft_error_exit(data, lst);
		close(lst->fd_str_rand);
	}
	else if (data->save_pipe)
	{
		if (dup2(data->save_pipe, STDIN_FILENO) == -1)
			ft_error_exit(data, lst);
		close(data->save_pipe);
	}
	ft_middle_execve(data, lst);
}

pid_t	ft_middle_fork(t_data *data, t_cmd *lst)
{
	pid_t	pid;

	signal(SIGINT, handle_sigint_fork);
	signal(SIGQUIT, handle_sigquit_fork);
	pid = fork();
	if (pid < 0)
		return (perror("fork middle failed :"), -1);
	else if (pid == 0)
		ft_middle_children(data, lst);
	return (pid);
}
