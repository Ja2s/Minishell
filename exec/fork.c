/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 15:12:43 by rasamad           #+#    #+#             */
/*   Updated: 2024/06/07 14:16:42 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_fd_heredoc(t_cmd *lst)
{

	lst->fd_str_rand = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, 0777);//open du fichier avec un nom random
	if (lst->fd_str_rand == -1) 
	{
		perror("Erreur lors de la création du fichier");
		return (-1);
	}
	if (lst->del_one == 1)
		return (write(lst->fd_str_rand, "\0", 1), 0);
	write(lst->fd_str_rand, lst->heredoc_content, ft_strlen(lst->heredoc_content));
	close(lst->fd_str_rand);
	return (0);
}

/*******************************************************************************************************/

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

void	handle_sigint_fork(int sig) 
{
	(void)sig;              // Pour éviter les avertissements de variable non utilisée
	printf("\n");
	g_sig = 1;
}

void	handle_sigquit_fork(int sig) 
{
	(void)sig;              // Pour éviter les avertissements de variable non utilisée
	g_sig = 2;
}

pid_t	ft_first_fork(t_data *data, t_cmd *lst)
{
	pid_t	pid;

	signal(SIGINT, handle_sigint_fork);
	signal(SIGQUIT, handle_sigquit_fork);
	pid = fork();
	if (pid < 0)
		return (perror("fork first failed :"), -1);
	if (pid == 0) //Children
	{
		//1. SI il y a un infile ET quil est en dernier  sil y en a un et quil est en dernier
		if (lst->redirecter && lst->fd_infile > 0 && lst->end_heredoc == 0)	// < f1
		{//recupere les donner dans fd_infile au lieu de lentree standard
			if (dup2(lst->fd_infile, STDIN_FILENO) == -1)
			{
				perror("dup2 first fd_infile failed : ");
				exit(EXIT_FAILURE);
			}
		}
		//2. SINON SI il y a un heredoc et quil est en dernier 
		if (lst->nb_del > 0 && lst->end_heredoc == 1)			// << eof
		{//appelle ft qui cp heredoc_content dans un fichier temporaire
			if (ft_fd_heredoc(lst) == -1)
				exit(EXIT_FAILURE);
			lst->fd_str_rand = open(".heredoc", O_RDONLY);
			if (dup2(lst->fd_str_rand, STDIN_FILENO) == -1)	//puis dup ce fd,  sinon dans stdin
			{
				perror("dup2 first fd_str_rand failed : ");
				exit(EXIT_FAILURE);
			}
			close(lst->fd_str_rand); // Fermer le descripteur de fichier heredoc
		}
		//1. SI OUTPUT envoi dans fd_outfile en prioriter
		if (lst->redirecter && lst->fd_outfile > 0)// > f2 || >> f2
		{
			if (dup2(lst->fd_outfile, STDOUT_FILENO) == -1)
			{
				perror("dup2 first fd_outfile failed : ");
				exit(EXIT_FAILURE);
			}
		}
		//2. SINON SI Y A UN NEXT envoi dans pipe[1], sinon dans stdout
		else if (lst->next) // --> | cmd
		{
			if (dup2(data->pipe_fd[1], STDOUT_FILENO) == -1)
			{
				perror("dup2 first pipe[1] failed : ");
				exit(EXIT_FAILURE);
			}
		}
		if (!lst->args[0] || data->exit_code == 127)
		{
			ft_lstclear(&data->cmd);
			free_env(data->mini_env);
			free_pipes(data->var.mini_env);
			free_pipes(data->var.pipes);
			exit(0);
		}
		if (ft_builtins(lst) != 0 || ft_builtins_env_fork(data, lst) != 0)
		{
			ft_lstclear(&data->cmd);
			free_env(data->mini_env);
			free_pipes(data->var.mini_env);
			free_pipes(data->var.pipes);
			exit(0);
		}
		execve(lst->path_cmd, lst->args, data->var.mini_env);
		
		perror("execve 1 : failed ");
		exit(EXIT_FAILURE);//comment retourner le bon code d'erreur en fonction du cas d'erreur ? Grace a la macro de waitpid WIFEXITED
	}
	return (pid);
}

/******************************************************************************************************/

pid_t	ft_middle_fork(t_data *data, t_cmd *lst)
{
	pid_t	pid;

	signal(SIGINT, handle_sigint_fork);
	signal(SIGQUIT, handle_sigquit_fork);
	pid = fork();
	if (pid < 0)
		return(perror("fork middle failed :"), -1);
	else if (pid == 0)
	{
		//1. SI il y a un infile ET quil est en dernier  sil y en a un et quil est en dernier
		if (lst->redirecter && lst->fd_infile > 0 && lst->end_heredoc == 0)// < f1
		{//recupere les donner dans fd_infile au lieu de lentree standard
			if (dup2(lst->fd_infile, STDIN_FILENO) == -1)
			{
				perror("dup2 middle fd_indile failed :");
				exit(EXIT_FAILURE);
			}
		}
		//2. SINON SI il y a un heredoc et est en dernier
		else if (lst->heredoc_content && lst->end_heredoc == 1)// << eof
		{//appelle ft qui cp heredoc_content dans un fichier temporaire
			if (ft_fd_heredoc(lst) == -1)
				exit(EXIT_FAILURE);
			lst->fd_str_rand = open(".heredoc", O_RDONLY);
			//puis recupere les donne du fd .heredoc au lieu de lentree standard
			if (dup2(lst->fd_str_rand, STDIN_FILENO) == -1)	//puis dup ce fd,  sinon dans stdin
			{
				perror("dup2 first fd_str_rand failed : ");
				exit(EXIT_FAILURE);
			}
			close(lst->fd_str_rand);
		}
		//3. SINON SI save pipe contient bien data pipe[0]
		else if (data->save_pipe)
		{//recupere les donnes dans save_pipe au lieu de lentree standard
			if (dup2(data->save_pipe, STDIN_FILENO) == -1)
			{
				perror("dup2 middle pipe_fd[0] failed :");
				exit(EXIT_FAILURE);
			}
		}
		//1. SI redirecteur != NULL et que fd_outfile a de la data
		if (lst->redirecter && lst->fd_outfile > 0)
		{//met la data de la sortie de lexecve dans fd_outfile
			if (dup2(lst->fd_outfile, STDOUT_FILENO) == -1)
			{
				perror("dup2 middle fd_outfile failed :");
				exit(EXIT_FAILURE);
			}
		}
		//2. SINON SI il y a bien un next
		else if (lst->next)
		{//met la data de la sortie de lexecve dans lextremiter decriture du pipe
			if (dup2(data->pipe_fd[1], STDOUT_FILENO) == -1)
			{
				perror("dup2 middle pipe[1] failed :");
				exit(EXIT_FAILURE);
			}
		}
		
		if (!lst->args[0] || data->exit_code == 127)
		{
			ft_lstclear(&data->cmd);
			free_env(data->mini_env);
			free_pipes(data->var.mini_env);
			free_pipes(data->var.pipes);
			exit(0);
		}
		if (ft_builtins(lst) != 0 || ft_builtins_env_fork(data, lst) != 0)
		{
			ft_lstclear(&data->cmd);
			free_env(data->mini_env);
			free_pipes(data->var.mini_env);
			free_pipes(data->var.pipes);
			exit(0);
		}
		if (data->exit_code != 0)
			exit(EXIT_SUCCESS);
		execve(lst->path_cmd, lst->args, data->var.mini_env);
		
		perror("execve 2 failed : ");
		//free lst->path et args
		exit(EXIT_FAILURE);
	}
	return (pid);
}

/********************************************************************************************************/

pid_t	ft_last_fork(t_data *data, t_cmd *lst)
{
	pid_t	pid;

	signal(SIGINT, handle_sigint_fork);
	signal(SIGQUIT, handle_sigquit_fork);
	pid = fork();
	if (pid < 0)
		return (perror("fork last failed :"), -1);
	if (pid == 0)
	{
		//1. prend soit dans infile en prioriter sil y en a un
		if (lst->redirecter && lst->fd_infile > 0 && lst->end_heredoc == 0)// < f1
		{
			if (dup2(lst->fd_infile, STDIN_FILENO) == -1)
			{
				perror("dup2 last fd_infile failed : ");
				exit(EXIT_FAILURE);
			}
		}
		//2. SINON SI HEREDOC  
		else if (lst->heredoc_content && lst->end_heredoc == 1)// << eof
		{
			if (ft_fd_heredoc(lst) == -1)	//apelle ft qui va open un fd et cp le heredoc dedans
				exit(EXIT_FAILURE);
			lst->fd_str_rand = open(".heredoc", O_RDONLY);
			if (dup2(lst->fd_str_rand, STDIN_FILENO) == -1)	//puis dup ce fd,  sinon dans stdin
			{
				perror("dup2 first fd_str_rand failed : ");
				exit(EXIT_FAILURE);
			}
			close(lst->fd_str_rand);
		}
		//3. soit dans pipe[0]
		else if (data->save_pipe)
		{
			if (dup2(data->save_pipe, STDIN_FILENO) == -1) // sortie de l'ancienne cmd
			{
				perror("dup2 last pipe_fd[0] failed : ");
				exit(EXIT_FAILURE);
			}
		}
		//1. envoi soit dans fd_outfile s'il y en a un soit dans stdout qui reste inchange (pas de dup2)
		if (lst->redirecter && lst->fd_outfile > 0)// > f2
		{
			if (dup2(lst->fd_outfile, STDOUT_FILENO) == -1)
			{
				perror("dup2 last fd_outfile failed : ");
				exit(EXIT_FAILURE);
			}
		}
		
		if (!lst->args[0] || data->exit_code == 127)
		{
			ft_lstclear(&data->cmd);
			free_env(data->mini_env);
			free_pipes(data->var.mini_env);
			free_pipes(data->var.pipes);
			exit(0);
		}
		if (ft_builtins(lst) != 0 || ft_builtins_env_fork(data, lst) != 0)
		{
			ft_lstclear(&data->cmd);
			free_env(data->mini_env);
			free_pipes(data->var.mini_env);
			free_pipes(data->var.pipes);
			exit(0);
		}
		execve(lst->path_cmd, lst->args, data->var.mini_env);
		perror("execve 3 : failed ");
		exit(EXIT_FAILURE);
	}
	return (pid);
}