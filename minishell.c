/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 11:02:31 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/15 18:08:42 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	ft_unset(t_env **mini_env, t_cmd *cmd)
{
	t_env	*tmp;
	t_env	*swap;
	int		i;

	i = 1;
	tmp = (*mini_env);
	while (cmd->args[i])
	{
		while (tmp->next && ft_strcmp(tmp->next->name, cmd->args[i]) != 0)
			tmp = tmp->next;
		if (tmp && ft_strcmp(tmp->next->name, cmd->args[i]) == 0)
		{
			swap = tmp->next;
			tmp->next = tmp->next->next;
			free(swap->name);
			free(swap->value);
			free(swap);
		}
		i++;
	}
	
}

int	ft_builtins_env(t_cmd *cmd, t_env *mini_env)
{
	if (ft_strcmp(cmd->args[0], "export") == 0)
	{
		ft_export(&mini_env, cmd);
			return (1);
	}
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
	{
		ft_unset(&mini_env, cmd);
		return (1);
	}
	else if (ft_strcmp(cmd->args[0], "env") == 0)
	{
		env_cmd(mini_env);
		return (1);
	}
	return (0);
}

int	launch_exec(t_data *data)
{
    int        	i;

	data->var.mini_env = ft_list_to_tab(data->mini_env);
	if (!data->var.mini_env)
		return (-1);
	data->save_pipe = 0;
    i = 0;
	int len_lst = ft_lstlen(data->cmd);
	if (ft_heredoc(data) == -1 || !data->cmd->args[0])
		return (-1);
	//ft_display_heredoc(data->cmd);
	while (data->cmd)
	{
		data->exit_code = 0;
		i++;
		if (data->cmd->redirecter)	//2. redirecter check
		ft_redirecter(data);
		if (data->cmd->next != NULL)		//3. Pipe check ne peut etre fait si 3 ou plus de cmd car il va refaire un pipe et erase lancien alors aue pour 2 cmd il fait qun pipe
			if (pipe(data->pipe_fd) == -1)
				exit_status(data, 1, "pipe failed\n");
		//cas ou la partie suivante ne doit pas etre faite, heredoc sans cmd, builtings
		if (ft_builtins_env(data->cmd, data->mini_env) == 0)
		{
			int check_access = ft_check_access(data);
			if (check_access == -1)  //4 Cmd check
				exit_status(data, 127, "");
			else if (check_access == -2)  //4 Cmd check
				exit_status(data, 127, "malloc error from [ft_check_access]");
			if (i == 1)
			{	//5. exec (cmd_first) | cmd_middle... | cmd_last
				if (ft_first_fork(data) == -1)
				if (data->pipe_fd[1] > 3)
					close(data->pipe_fd[1]);// je close lecriture pour pas que la lecture attende indefinement.
				data->save_pipe = data->pipe_fd[0]; //je save la lecture pour le next car je vais re pipe pour avoir un nouveau canal 
			}
			else if (i < len_lst)
			{	//6. exec cmd_first | (cmd_middle...) | cmd_last
				ft_middle_fork(data);
				close(data->pipe_fd[1]);
				data->save_pipe = data->pipe_fd[0];
			}
			else if (i == len_lst)
			{	//7. exec  exec cmd_first | cmd_middle... | (cmd_last)
				ft_last_fork(data);
				close(data->pipe_fd[0]);
			}
		}
		ft_close(data->cmd);
		data->cmd = data->cmd->next;
	}
	printf("EXIT_STATUS = %d\n", data->exit_code);
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
		return (printf("Malloc error\n"), -1);
	while (1)
	{
		if (prompt_customer(&data) == 0)
		{
			if (data.var.rl[0] != '\0' && syntaxe_error(&data, data.var.rl) == 0)
			{
				if (parser(&data) == 0)
					if (final_parse(&data) == -1)
					{
						ft_lstclear(&data.cmd);
						free_pipes(data.var.pipes);
						free(data.var.pwd);
					}
			}
		}
	}
	rl_clear_history();
}

//launch_exec | bultins

//faire < (syntaxe err) puis ls