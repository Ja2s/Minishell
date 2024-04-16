/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 15:42:26 by rasamad           #+#    #+#             */
/*   Updated: 2024/04/05 11:18:14 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	display_lst(t_lst *lst);
static void	ft_init_list(t_lst **lst, char **av);

int	main(int ac, char **av, char **envp)
{
	t_lst	*lst;
	t_lst	*begin_lst;
	t_struct	var;
	int		i;

	i = 0;
	lst = NULL;
	if (ac)
		if(av[1] == NULL)
			return (0);
	ft_init_list(&lst, av);
	int len_lst = ft_lstlen(lst);
	begin_lst = lst;

	while (lst)
	{
		i++;
		ft_printf("command %d\n", i);
		display_lst(lst);

		if (lst->heredoc > 0)		//1. Heredoc check
			printf("ft_heredoc() a faire\n");
		lst->open = 0;
		if (lst->redirection)		//2. Redirection check
			ft_redirection(lst);

		if (lst->next != NULL)		//3. Pipe check ne peut etre fait si 3 ou plus de cmd car il va refaire un pipe et erase lancien alors aue pour 2 cmd il fait qun pipe 
		{
			if (pipe(var.pipe_fd) == -1){
				perror("pipe failed");
				exit(EXIT_FAILURE);
			}
		}

		ft_check_access(lst, envp);	//4 Cmd check

		if (i == 1 && lst->open == 0){			//5. exec (cmd_first) | cmd_middle... | cmd_last
			ft_printf("go exec first cmd\n\n");
			ft_first_fork(lst, &var, envp);
			close(var.pipe_fd[1]);// je close lecriture pour pour pas que la lecture attendent indefinement.
			var.save_pipe = var.pipe_fd[0]; //je save la lecture pour le next car je vais re pipe pour avoir un nouveau canal 
		}

		else if (i < len_lst && lst->open == 0){//6. exec cmd_first | (cmd_middle...) | cmd_last
			ft_printf("go exec middle cmd\n\n");
			ft_middle_fork(lst, &var, envp);
			close(var.pipe_fd[1]);
			var.save_pipe = var.pipe_fd[0];
		}

		else if (i == len_lst && lst->open == 0){//7. exec  exec cmd_first | cmd_middle... | (cmd_last)
			ft_printf("go exec last cmd\n\n");
			
			ft_last_fork(lst, &var, envp);
			close(var.pipe_fd[0]);
			}

		//ft_free_token(lst);
		ft_close(lst);
		lst = lst->next;
	}
	ft_free_lst(begin_lst);
	return (0);
}

int	ft_lstlen(t_lst *elem)
{
	int	i;

	i = 0;
	while (elem)
	{
		i++;
		elem = elem->next;
	}
	return (i);
}

static void	display_lst(t_lst *elem)
{
	//ft_printf(">\t\t|%s|\n", elem->line);
	//ft_printf("heredoc =\t%d\ndelimiter =\t|%s|\n", elem->heredoc, elem->delimiter);
	ft_printf("lst =\t\t|%p|\n", elem);
	ft_printf("cmd =\t\t|%s|\n", elem->cmd);
	if (elem->args)
		for (size_t i = 0; elem->args[i] ; i++)
			ft_printf("args[%d] =\t|%s|\n", i, elem->args[i]);
	if (elem->redirection)
		for (size_t i = 0; elem->redirection[i] ; i++)
			ft_printf("redi[%d] =\t|%s|\n", i, elem->redirection[i]);
	ft_printf("next =\t\t|%p|\n", elem->next);
}

static void	ft_init_list(t_lst **lst, char **av)//test de ligne de commande (sortie du parsing)
{
	t_lst	*first_elem;
	t_lst	*second_elem;
	//t_lst	*third_elem;
	//t_lst	*fourth_elem;

	first_elem = ft_lstnew_minishell();
	ft_lstadd_back_minishell(lst, first_elem);

	first_elem->heredoc = 0;
	first_elem->cmd = av[1];

	first_elem->args = malloc(3 * sizeof(char *));
	if (!first_elem->args){
		ft_printf("malloc args failed\n");
		exit(EXIT_FAILURE);
	}
	first_elem->args[0] = av[1];
	first_elem->args[1] = "f2";
	first_elem->args[2] = NULL;

/*	first_elem->redirection = malloc(2 * sizeof(char *));
	if (!first_elem->redirection){
		ft_printf("malloc redi failed\n");
		exit(EXIT_FAILURE);
	}
	first_elem->redirection[0] = "> f1";
	first_elem->redirection[1] = ">> f11";
	first_elem->redirection[1] = NULL;*/
	first_elem->redirection = NULL;

	first_elem->next = NULL;


/*******************************************************************************************************/

	second_elem = ft_lstnew_minishell();
	ft_lstadd_back_minishell(lst, second_elem);

	second_elem->heredoc = 0;
	second_elem->cmd = av[2];
	
	second_elem->args = malloc(2 * sizeof(char *));
	if (!second_elem->args){
		ft_printf("malloc args failed\n");
		exit(EXIT_FAILURE);
	}
	second_elem->args[0] = av[2];
	second_elem->args[1] = NULL;

	second_elem->redirection = malloc(8 * sizeof(char *));
	if (!second_elem->redirection)
	{
		ft_printf("malloc failed\n");
		exit(EXIT_FAILURE);
	}
	second_elem->redirection[0] = "> f3";
	second_elem->redirection[1] = "> f4";
	second_elem->redirection[2] = "> f4";
	second_elem->redirection[3] = "> f4";
	second_elem->redirection[4] = "> f4";
	second_elem->redirection[5] = "> f4";
	second_elem->redirection[6] = "> f4";
	second_elem->redirection[7] = NULL;
	//second_elem->redirection = NULL;
	second_elem->next = NULL;

/***********************************************************************************************************************/

	/*third_elem = ft_lstnew_minishell();
	ft_lstadd_back_minishell(lst, third_elem);

	third_elem->heredoc = 0;
	third_elem->cmd = av[3];

	third_elem->args = malloc(3 * sizeof(char *));
	third_elem->args[0] = av[3];
	third_elem->args[1] = "-e";
	third_elem->args[2] = NULL;
	//third_elem->args = NULL;

	third_elem->redirection = malloc(2 * sizeof(char *));
	if (!third_elem->redirection)
	{
		ft_printf("malloc failed\n");
		exit(EXIT_FAILURE);
	}
	third_elem->redirection[0] = ">> f3";
	third_elem->redirection[1] = NULL;
	third_elem->redirection = NULL;

	third_elem->next = NULL;*/

	/**********************************************************************************************************************/

	/*fourth_elem = ft_lstnew_minishell();
	ft_lstadd_back_minishell(lst, fourth_elem);

	fourth_elem->heredoc = 0;
	fourth_elem->cmd = av[4];

	fourth_elem->args = malloc(3 * sizeof(char *));
	fourth_elem->args[0] = av[4];
	fourth_elem->args[1] = NULL;
	//fourth_elem->args[2] = NULL;

	fourth_elem->redirection = malloc(2 * sizeof(char *));
	if (!fourth_elem->redirection)
	{
		ft_printf("malloc failed\n");
		exit(EXIT_FAILURE);
	}
	fourth_elem->redirection[0] = ">       f4";
	fourth_elem->redirection[1] = NULL;
	//fourth_elem->redirection = NULL;
	
	fourth_elem->next = NULL;*/
}