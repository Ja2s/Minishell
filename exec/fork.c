/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gavairon <gavairon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 15:12:43 by rasamad           #+#    #+#             */
/*   Updated: 2024/05/16 01:20:32 by gavairon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_fd_rand_heredoc(t_cmd *lst)
{
	char str_rand[6 + 1]; // +1 pour le caractère nul
	char charset[] = "aBcDeFgHiJkLmNoPqRsTuVwXyZ0123456789";
	int fd_rand;
	int	i;

	fd_rand = open("/dev/urandom", O_RDONLY);
	if (fd_rand == -1) 
	{
		perror("Erreur lors de l'ouverture de /dev/urandom");
		return 1;
	}
	if (read(fd_rand, str_rand, 6) == -1)//read dans str_rand 5 caractere de dev/urandom
	{
		perror("Erreur lors de la lecture de /dev/urandom");
		close(fd_rand);
		return 1;
	}
	close(fd_rand);
	//str_rand[0] = '.';
	i = 0;
	while (i < 6)
	{
		str_rand[i] = charset[str_rand[i] % (sizeof(charset) - 1)];
		i++;
	}
	str_rand[6] = '\0';
	lst->fd_str_rand = open(str_rand, O_CREAT | O_RDWR, 0777);//open du fichier avec un nom random
	if (lst->fd_str_rand == -1) {
		perror("Erreur lors de la création du fichier");
		return 1;
	}
	i = 0;
	while (lst->heredoc_content[i])
	{
		write(lst->fd_str_rand, lst->heredoc_content[i], ft_strlen(lst->heredoc_content[i]));
		write(lst->fd_str_rand, "\n", 1);
		i++;
	}
	return 0;
}

int	ft_fd_heredoc(t_cmd *lst)
{
	int	i;

	lst->fd_str_rand = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, 0777);//open du fichier avec un nom random
	if (lst->fd_str_rand == -1) 
	{
		perror("Erreur lors de la création du fichier");
		return -1;
	}
	i = 0;
	while (lst->heredoc_content[i])
	{
		write(lst->fd_str_rand, lst->heredoc_content[i], ft_strlen(lst->heredoc_content[i]));
		write(lst->fd_str_rand, "\n", 1);
		i++;
	}
	close(lst->fd_str_rand);
	return 0;
}


/*******************************************************************************************************/

int	ft_first_fork(t_data *data)
{
	pid_t	pid;
	t_cmd	*lst;

	pid = fork();
	if (pid < 0)
	{
		perror("fork first failed :");	
		exit(EXIT_FAILURE);
	}
	if (pid == 0) //Children
	{
		lst = data->cmd;
		//1. SI il y a un infile ET quil est en dernier  sil y en a un et quil est en dernier
		if (lst->redirecter && lst->fd_infile > 0 && lst->end_heredoc == 0)// < f1
		{//recupere les donner dans fd_infile au lieu de lentree standard
			if (dup2(lst->fd_infile, STDIN_FILENO) == -1)
			{
				perror("dup2 first fd_infile failed : ");
				exit(EXIT_FAILURE);
			}
		}
		//2. SINON SI il y a un heredoc et quil est en dernier 
		else if (lst->heredoc_content && lst->end_heredoc == 1)// << eof
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
		if (ft_builtins(lst) != 0)
			exit(0);
		if (data->exit_code != 0)
			exit(EXIT_SUCCESS);
		execve(lst->path_cmd, lst->args, data->var.mini_env);
		perror("execve 1 : failed ");
		exit(EXIT_FAILURE);//comment retourner le bon code d'erreur en fonction du cas d'erreur ? Grace a la macro de waitpid WIFEXITED
	}
	else if (pid > 0)
	{
		int status;
    	waitpid(pid, &status, 0);
		
		if (status == 0)
			return (-1);
		if (WIFEXITED(status)) {//execve failed
			printf("Command failed with exit status: %d\n", WIFEXITED(status));
			// Ici, vous pouvez ajuster votre logique en fonction de exit_status
			exit_status(data, WIFEXITED(status), "");//Pourquoi une fois echo $? apres cmd not perm il affiche pas exit_stat qui semble etre bien a 1, cest pcq je fait pas return -1`
			return (-1);
		}
		if (WIFSIGNALED(status)) {
			int signal_number = WTERMSIG(status);
			printf("Command terminated by signal: %d\n", signal_number);
			// Ici, vous pouvez ajuster votre logique en fonction du signal reçu
		}
	}
	return (0);
}

/******************************************************************************************************/

int	ft_middle_fork(t_data *data)
{
	pid_t	pid;
	t_cmd *lst;

	pid = fork();
	if (pid < 0)
	{
		perror("fork middle failed :");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		lst = data->cmd;
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
		if (lst->next)
		{//met la data de la sortie de lexecve dans lextremiter decriture du pipe
			if (dup2(data->pipe_fd[1], STDOUT_FILENO) == -1)
			{
				perror("dup2 middle pipe[1] failed :");
				exit(EXIT_FAILURE);
			}
		}
		if (ft_builtins(lst) != 0)
			exit(0);
		if (data->exit_code != 0)
			exit(EXIT_SUCCESS);
		execve(lst->path_cmd, lst->args, data->var.mini_env);
		perror("execve 2 failed : ");
		//free lst->path et args
		exit(EXIT_FAILURE);
	}
	else
	{
		int status;
    	waitpid(pid, &status, 0);

		if (status == 0)
			return (-1);
		if (WIFEXITED(status)) {//execve failed
			printf("Command failed with exit status: %d\n", WIFEXITED(status));
			// Ici, vous pouvez ajuster votre logique en fonction de exit_status
			exit_status(data, WIFEXITED(status), "");//Pourquoi une fois echo $? apres cmd not perm il affiche pas exit_stat qui semble etre bien a 1, cest pcq je fait pas return -1`
			return (-1);
		}
		if (WIFSIGNALED(status)) {
			int signal_number = WTERMSIG(status);
			printf("Command terminated by signal: %d\n", signal_number);
			// Ici, vous pouvez ajuster votre logique en fonction du signal reçu
		}
	}
	return (0);
}

/********************************************************************************************************/

int	ft_last_fork(t_data *data)
{
	pid_t	pid;
	t_cmd	*lst;

	pid = fork();
	if (pid < 0){
		perror("fork last failed :");	
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		lst = data->cmd;
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
		if (ft_builtins(lst) != 0)
			exit(0);
		if (data->exit_code != 0)
			exit(EXIT_SUCCESS);
		execve(lst->path_cmd, lst->args, data->var.mini_env);
		perror("execve 3 : failed ");
		exit(EXIT_FAILURE);
	} 
	else if (pid > 0)
	{
		int status;
    	waitpid(pid, &status, 0);
		
		if (status == 0)// ne pas changer exit_code soit pcq il a deja etais modif en amont avec une err soit pcq tout est good
			return (-1);
		if (WIFEXITED(status)) {//execve failed
			printf("Command failed with exit status: %d\n", WIFEXITED(status));
			// Ici, vous pouvez ajuster votre logique en fonction de exit_status
			exit_status(data, WIFEXITED(status), "");//Pourquoi une fois echo $? apres cmd not perm il affiche pas exit_stat qui semble etre bien a 1, cest pcq je fait pas return -1`
			return (-1);
		}
		if (WIFSIGNALED(status)) {
			int signal_number = WTERMSIG(status);
			printf("Command terminated by signal: %d\n", signal_number);
			// Ici, vous pouvez ajuster votre logique en fonction du signal reçu
		}
	}
	return (0);
}