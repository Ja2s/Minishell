/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 11:02:31 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/11 17:07:55 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

// Définition de la variable globale
volatile sig_atomic_t g_sig = 0;

int	ft_check_num(t_data *data)
{
	int	i;
	//not + ou not - ou not inverse de compris entre 0 et 9
	if (data->cmd->args[1][0] != '+' && data->cmd->args[1][0] != '-' && \
	(!(data->cmd->args[1][0] >= '0' && data->cmd->args[1][0] <= '9')))
		return(-1);
	if (data->cmd->args[1][0] == '+' \
	&& (!(data->cmd->args[1][1] >= '0' && data->cmd->args[1][1] <= '9')))
		return(-1);	// just +
	else if (data->cmd->args[1][0] == '-' && \
	(!(data->cmd->args[1][1] >= '0' && data->cmd->args[1][1] <= '9')))
		return(-1);	// just -
	i = 1;
	while(data->cmd->args[1][i])
	{
		if ((!(data->cmd->args[1][i] >= '0' && data->cmd->args[1][i] <= '9')))
			return(-1);
		i++;
	}
	return (0);
}

int	ft_exit_prog(t_data *data)
{
	int exit_stat = 0;
	printf("exit\n");
	if (data->cmd->args[1] && ft_check_num(data) == -1)
	{
		printf("minishell: exit: %s: numeric argument required\n", data->cmd->args[1]);
		ft_lstclear(&data->cmd);
		free_pipes(data->var.pipes);
		free_pipes(data->var.mini_env);
		free_env(data->mini_env);
		rl_clear_history();
		exit(2);
	}
	if (data->cmd->nb_args > 2)
		return(exit_status(data, 1, "minishell: exit: too many arguments\n"), -1);
	if (data->cmd->args[1])
		exit_stat = ft_atoi(data->cmd->args[1]) % 256; // Convert argument to exit status
	ft_lstclear(&data->cmd);
	free_pipes(data->var.pipes);
	free_pipes(data->var.mini_env);
	free_env(data->mini_env);
	rl_clear_history();
	exit(exit_stat);
	return (0);
}

int	ft_stat_check(int check_access, t_data *data, t_cmd *lst)
{
	struct stat statbuf;
	if (check_access == -2)
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

int	launch_exec(t_data *data)
{
	int		i;
	t_cmd	*lst;

	// Check if the command is "exit" and handle it before anything else
	if (data->cmd->args[0] && data->cmd->next == NULL && ft_strcmp(data->cmd->args[0], "exit") == 0)
		return(ft_exit_prog(data));
	lst = data->cmd;
	data->var.mini_env = ft_list_to_tab(data->mini_env);
	if (!data->var.mini_env)
		return (exit_status(data, 1, "malloc error from [list_to_tab]\n"), -1);
	data->save_pipe = 0;
	i = 0;
	int len_lst = ft_lstlen(lst);
	if (ft_heredoc(data) == -1)
		return (ft_free_all_heredoc(data->cmd), -1);
	while (lst)
	{
		data->exit_code = 0;
		i++;
		if (lst->redirecter)	//2. redirecter check
			ft_redirecter(data, lst);
		if (lst->next != NULL)		//3. Pipe check ne peut etre fait si 3 ou plus de cmd car il va refaire un pipe et erase lancien alors aue pour 2 cmd il fait qun pipe
			if (pipe(data->pipe_fd) == -1)
				exit_status(data, 1, "pipe failed\n");
		if (!lst->args[0] && !lst->next)
			return (ft_close_pipe(data), ft_free_all_heredoc(data->cmd), -1);
		if (ft_builtins_env(lst, data, i) == 0)// pas de builtins
		{
			if (ft_is_builtins_no_access(lst) == 0)
				if (ft_stat_check(ft_check_access(data, lst), data, lst) == -1)
					return(ft_free_all_heredoc(data->cmd), ft_close_pipe(data), -1);
			if (i == 1)
			{	//5. exec (cmd_first) | cmd_middle | ... | cmd_last
				ft_first_fork(data, lst);
				close(data->pipe_fd[1]);//close evite boucle infini
				data->save_pipe = data->pipe_fd[0];
			}
			else if (i < len_lst)
			{	//6. exec cmd_first | (cmd_middle | ...) | cmd_last
				ft_middle_fork(data, lst);
				close(data->pipe_fd[1]);
				close(data->save_pipe);
				data->save_pipe = data->pipe_fd[0];
			}
			else if (i == len_lst)
			{	//7. exec cmd_first | cmd_middle | ... | (cmd_last)
				ft_last_fork(data, lst);
				close(data->save_pipe);
				//close(data->pipe_fd[1]);
			}
		}
		ft_close(lst);
		lst = lst->next;
	}
	ft_close_pipe(data);
	i = 0;
	while (i < len_lst){
		int status;
		status = 0;
		waitpid(-1, &status, 0);
	
		if (g_sig == 1){
			exit_status(data, 130, "");
			g_sig = 0;
		}
		if (g_sig == 2){
			exit_status(data, 131, "Quit (core dumped)\n");
			g_sig = 0;
		}
		if (WIFEXITED(status) && status != 0) //execve failed
			exit_status(data, WIFEXITED(status), "");
		i++;
	}
	free_pipes(data->var.mini_env);
    data->var.mini_env = NULL;
	ft_free_all_heredoc(data->cmd);
	if (data->exit_code != 0)
		return (-1);
	return (0);
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
