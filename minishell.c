/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 11:02:31 by jgavairo          #+#    #+#             */
/*   Updated: 2024/04/29 13:54:33 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	launch_exec(t_cmd **lst, char **envp)
{
    t_struct   	*var;
    int        	i;

	var = malloc(sizeof(t_struct));
	var->pipe_fd[0] = 0;
	var->pipe_fd[1] = 0;
	var->save_pipe = 0;
    i = 0;
    int len_lst = ft_lstlen((*lst));
    while (*lst)
    {
        i++;
        (*lst)->open = 0;
        if ((*lst)->redirecter)        //2. redirecter check
            ft_redirecter(*lst);
        if ((*lst)->next != NULL)        //3. Pipe check ne peut etre fait si 3 ou plus de cmd car il va refaire un pipe et erase lancien alors aue pour 2 cmd il fait qun pipe 
        {
            if (pipe(var->pipe_fd) == -1){
                perror("pipe failed");
                exit(EXIT_FAILURE);
            }
        }
		//cas ou la partie suivante ne doit pas etre faite, heredoc sans cmd, builtings
        ft_check_access((*lst), envp);    //4 Cmd check

        if (i == 1 && (*lst)->open == 0)
		{   //5. exec (cmd_first) | cmd_middle... | cmd_last
            //printf("go exec first cmd\n\n");
            ft_first_fork((*lst), &var, envp);
			if (var->pipe_fd[1] > 3)
            	close(var->pipe_fd[1]);// je close lecriture pour pour pas que la lecture attendent indefinement.
            var->save_pipe = var->pipe_fd[0]; //je save la lecture pour le next car je vais re pipe pour avoir un nouveau canal 
        }

        else if (i < len_lst && (*lst)->open == 0)
		{	//6. exec cmd_first | (cmd_middle...) | cmd_last
            //printf("go exec middle cmd\n\n");
            ft_middle_fork((*lst), &var, envp);
            close(var->pipe_fd[1]);
            var->save_pipe = var->pipe_fd[0];
        }

        else if (i == len_lst && (*lst)->open == 0)
		{	//7. exec  exec cmd_first | cmd_middle... | (cmd_last)
            //printf("go exec last cmd\n\n");
            ft_last_fork((*lst), &var, envp);
            close(var->pipe_fd[0]);
        }
        //ft_free_token(lst);
        ft_close(*lst);
        (*lst) = (*lst)->next;
    }
	free(var);
    return (0);
}

int main(int argc, char **argv, char **envp)
{
	char	*rl;
	//char	*pwd;
	char	**input;
	char	**pipes;
	int		i;
	t_var	*var;
	t_cmd	*cmd = NULL;
	t_cmd	*tmp = NULL;
	(void)argc;
	(void)argv;
	var = malloc(sizeof(t_var));
	if (!var)
		printf("t_var memory allocation Error\n");
	var->mini_env = NULL;
	pipes = NULL;
	rl = NULL;
	i = 0;
	printf_title();
	while(1)
	{
		//pwd = getcwd(NULL, 0);	//je recupere le chemin d'acces pour l'afficher tel fish
		//var->mini_env = env_copyer(envp, var); // je recupere et copie lenvironnement dans un autre tableau
		//printf ("\033[90m%s\033[0m", pwd);	//je l'affiche
		rl = readline("\e[33m$> \e[37m");	//je recupere la ligne en entree dans une boucle infini afin de l'attendre
		printf("==========RL : %s\n", rl);
		if (syntaxe_error(rl) == 0)	//je check les erreurs de syntaxes et lance le programme seulement si tout est OK
		{
			printf("RL = \e[31m%s\n\e[0m", rl);
			negative_checker(rl);		//je check toutes mes cotes et passe en negatif tout ce qui ne vas pas devoir etre interprete
			printf("RL after cotes checker = \e[31m%s\n\e[0m", rl);
			rl = dolls_expander(rl);
			printf("RL  after expander = \e[31m%s\n\e[0m", rl);
			//if (pipes_counter(rl) == 1)
			pipes = ft_split(rl, '|');	//je separe chaque commande grace au pipe
			// else
			// {
			// 	pipes = malloc(sizeof(char *) * 2);
			// 	pipes[0] = ft_strdup(rl);
			// 	pipes[1] = NULL;
			// }
			add_history(rl);
			free(rl);
			while (pipes[i])	//chaque commande est rangee par ligne dans un tableau afin de les traiter une a une 
			{
				tmp = ft_lstnew_minishell(); //jinitialise ma structure car nous allons en avoir besoin
				if (!tmp)
					break;
				if (heredoc_checker(pipes[i], &tmp) == -1)
					printf("Heredoc memory error\n");
				redirecter(pipes[i], &tmp);	//je vais chercher toutes les redirects afin de les ranger dans une variable a part
				pipes[i] = redirect_deleter(pipes[i]);	//je vai supprimer toutes les redirects de la ligne principale afin de ne plus les prendre en compte
				input = ft_split(pipes[i], ' ');	//il me reste donc plus que la commande et les arguments separes d'espaces, je les separe donc et les range dans input
				if (stock_input(input, &tmp) == 0)	//je vais recuperer le nom de la commande et les arguments dans input et les ranger dans la structure tmp
				{
					i++;	// la commande a ete traitee, je passe a la prochaine (si il y en a une)
					ft_lstadd_back_minishell(&cmd, tmp);	//j'ajoute le noeud tmp ou tout a ete ranger a ma liste chainee 
				}
			}
			i = 0;
			ft_printf_struct(cmd);
			//start = cmd;
			printf("\033[38;5;220mLancement de Launch_exec...\033[0m\n");
			if (launch_exec(&cmd, envp) == -1)
				printf ("Error exec\n");
			printf("--------------------------------------------------------\n");
			ft_lstclear(&cmd);
			free_pipes(pipes);
			//free(pwd);
		}
	}
	rl_clear_history();
}
