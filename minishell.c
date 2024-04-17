/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gavairon <gavairon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 11:02:31 by jgavairo          #+#    #+#             */
/*   Updated: 2024/04/17 14:21:33 by gavairon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*ft_lstlast(t_cmd *lst)
{
	t_cmd	*last;

	if (!lst)
		return (NULL);
	last = lst;
	while (last->next)
		last = last->next;
	return (last);
}

void	args_free(t_cmd *lst)
{
	int	i;

	i = 0;
	while (i < lst->nb_args)
	{
		free(lst->args[i]);
		i++;
	}
	if (lst->nb_args > 0)
		free(lst->args);
}

void	red_free(t_cmd *lst)
{
	int	i;

	i = 0;
	while (i < lst->nb_red)
	{
		free(lst->redirecter[i]);
		i++;
	}
	if (lst->nb_red > 0)
		free(lst->redirecter);
}

void	del_free(t_cmd *lst)
{
	int	i;

	i = 0;
	while (i < lst->nb_del)
	{
		free(lst->delimiter[i]);
		i++;
	}
	if (lst->nb_del > 0)
		free(lst->delimiter);
}

void	ft_lstdelone(t_cmd *lst)
{
	args_free(lst);
	red_free(lst);
	del_free(lst);
	if (lst)
		free(lst);
}

void	ft_lstclear(t_cmd **lst)
{
	t_cmd	*tmp;

	while (*lst)
	{
		tmp = (*lst)->next;
		ft_lstdelone(*lst);
		*lst = tmp;
	}
}

int	ft_lstlen(t_cmd *elem)
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

int	double_cote_checker(char *rl, bool *cot, size_t *i)
{
	size_t	p;

	*cot = false;
	p = *i;
	while (rl[p] && *cot == false)
	{
		p++;
		if (rl[p] == 34)
			*cot = true;
	}
	if (*cot == false)
		return (-1);
	*i = p;
	return (0);
}

int	simple_cote_checker(char *rl, bool *cot, size_t *i)
{
	size_t	p;

	*cot = false;
	p = *i;
	while (rl[p] && *cot == false)
	{
		p++;
		if (rl[p] == 39)
			*cot = true;
	}
	if (*cot == false)
		return (-1);
	*i = p;
	return (0);
}

/*Cette fonction vas checker que toutes les cotes sont bien et si ce n'est pas le cas, provoque une erreur de syntaxe*/
int	cote_checker(char *rl)
{
	size_t	i;
	bool	cot;

	cot = true;
	i = 0;
	while (rl[i])
	{
		if (rl[i] == 34)
		{
			if (double_cote_checker(rl, &cot, &i) == -1)
				return (-1);
		}
		if (rl[i] == 39)
		{
			if (simple_cote_checker(rl, &cot, &i) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}

/*Cette fonction vas checker qu'aucun double pipe n'exite dans lentree, si c'est le cas, retourne une erreur de syntaxe*/
int	double_pipe_checker(char *rl)
{
	size_t	i;

	i = 0;
	while (rl[i])
	{
		if (rl[i] == '|' && rl[i + 1] == '|')
			return (-1);
		i++;
	}
	return (0);
}

/*Cette fonction bas checker les erreurs de syntaxes lie aux chevrons (exemple "<< <", ">>>" ou avec aucun nom de fichier)*/
int	rafters_checker(char *rl)
{
	int	i;

	i = 0;
	while (rl[i])
	{
		if (rl[i] == '<' || rl[i] == '>')
		{
			i++;
			if (rl[i] == '<' || rl[i] == '>')
				i++;
			while (rl[i] == ' ')
				i++;
			if (rl[i] == '\0' || rl[i] == '<' || rl[i] == '>' || rl[i] == '|')
				return (-1);
		}
		else
			i++;
	}
	return (0);
}

/*Cette fonction vas lancer chaque sous fonction de check pour les erreurs de syntaxes (fonctions plus hautez)*/
int	syntaxe_error(char	*rl)
{
	if (rafters_checker(rl) == -1)
		return (printf("\033[31mSyntaxe error\n\033[0m"), -1);
	if (cote_checker(rl) == -1)
		return (printf("\033[31mSyntaxe error\n\033[0m"), -1);
	if (double_pipe_checker(rl) == -1)
		return (printf("\033[31mSyntaxe error\n\033[0m"), -1);
	return (0);
}

/*Cette fonction vas parcourir toute lentree, et passer en negatif les caracteres speciaux entre cotes, suivant ceux que nous voulons interpreter ou pas*/
void	negative_checker(char *rl)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(rl);
	while (i < len)
	{
		if (rl[i] == 34)
		{
			i++;
			while (rl[i] != 34)
			{
				if (rl[i] == ' ' || rl[i] == '<' || rl[i] == '>')
					rl[i] = rl[i] * -1;
				i++;
			}
		}
		if (rl[i] == 39)
		{
			i++;
			while (rl[i] != 39)
			{
				if (rl[i] == ' ' || rl[i] == '$' || rl[i] == '<' || rl[i] == '>' || rl[i] == '|' || rl[i] == 34)
				rl[i] = rl[i] * -1;
				i++;
			}
		}
		i++;
	}
}

/*Cette fonction vas chercher les infos dans input, et les ranger dans le noeud de la structure*/
void	command_stocker(char **input, t_cmd **cmd)
{
	int	i;

	i = 0;
	if (input[0][0] != '<' && input[0][0] != '>')
		(*cmd)->args[i] = ft_strdup(input[i]);
	i++;
	while (input[i])
	{
		(*cmd)->args[i] = ft_strdup(input[i]);
		i++;
	}
}

/*Cette fonction vas aller check le nombre d'arguments qui sont present dans la commande pour allouer la bonne taille au tableau associe*/
int	args_memory_alloc(char **input, t_cmd **cmd)
{
	int	i;

	i = 0;
	while (input[i])
		i++;
	(*cmd)->nb_args = i;
	(*cmd)->args = malloc(sizeof(char *) * (*cmd)->nb_args);
	if (!(*cmd)->args)
		return (-1);
	return (0);
}

/*Cette fonction vas apper la fonction plus haute, (de base cetait plus propre et plus logique de l'avoir vu que je faisai pariel avec les flags)*/
int	memory_alloc(char **input, t_cmd **cmd)
{
	if (args_memory_alloc(input, cmd) == -1)
		return (-1);
	return (0);
}

/*Cette fonction vas donc allouer la taille necessaire et ensuite aller stocker la commande dans le noeud*/
int	stock_input(char **input, t_cmd **cmd)
{
	if (memory_alloc(input, cmd) == -1)
		return (-1);
	command_stocker(input, cmd); 
	return (0);
}

/*Fonction de deboggage, sert a print la structure afin de savoir ou j'en suis*/
int	ft_printf_struct(t_cmd *cmd)
{
	int i;
	i = 0;
	while (cmd)
	{
		printf("\n\n\n\033[33m-------------Command name----------------\033[0m\n");
		printf("Commande -> |%s|\n", cmd->args[i]);
		i++;
		printf("\033[33m-------------Arguments----------------\033[0m\n");
		while (i < cmd->nb_args)
			{
				printf("Arg n%d -> |%s|\n", i + 1, cmd->args[i]);
				i++;
			}
		i = 0;
		if (cmd->nb_red > 0)
		{
			printf("\033[33m-------------Redirections----------------\033[0m\n");
			while(i < cmd->nb_red)
			{
				printf("Redirecter -> |%s|\n", cmd->redirecter[i]);
				i++;
			}
		}
		i = 0;
		if (cmd->heredoc == true)
		{
			printf("\033[35m-------------Heredoc (true)----------------\033[0m\n");
			while (i < cmd->nb_del)
			{
				printf("delimiter -> |%s|\n", cmd->delimiter[i]);
				i++;
			}
		}
		i = 0;
		

		printf("\033[35m-------------End of command----------------\033[0m\n");
		if (cmd->next)
			cmd = cmd->next;
		else
			return (-1);
	}
	return (0);
}

/*Cette fonction vas servir a checker le nombre de redirecteurs present dans la commande pour pouvoir allouer la bonne taille au tableau de redirection*/
int	redirect_counter(char *pipes)
{
	int	i;
	int	len;

	len = 0;
	i = 0;
	while (pipes[i])
	{
		if (pipes[i] == '<' || pipes[i] == '>')
		{
			if (pipes[i] == '<' && pipes[i + 1] == '<')
				i = i + 2;
			else
			{
				len++;
				i++;
				if (pipes[i] == '<' || pipes[i] == '>')
					i++;
			}
		}
		else
			i++;
	}
	return (len);
}

//compter le nb de heredoc pour sqvoir le nb de lignes dans le tab delimiter <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
int	heredoc_counter(char *pipes)
{
	int	i;
	int	heredoc;

	heredoc = 0;
	i = 0;
	while (pipes[i])
	{
		if (pipes[i] && pipes[i] == '<')
		{
			i++;
			if (pipes[i] && pipes[i] == '<')
			{
				heredoc++;
				i++;
			}
		}
		else
			i++;
	}
	return (heredoc);
}

int	heredoc_memory_allocer(char *pipes, t_cmd **cmd)
{
	(*cmd)->nb_del = heredoc_counter(pipes);
	if ((*cmd)->nb_del > 0)
		(*cmd)->delimiter = malloc(sizeof(char*)* (*cmd)->nb_del);
	if (!(*cmd)->delimiter && (*cmd)->nb_del > 0)
		return (-1);
	return (0);
}

int heredoc_copyer(char *pipes, t_cmd **cmd, int *i, int del)
{
	int start = 0;
	int p = *i;
	if (pipes[p] && pipes[p] == '<')
	{
		(*cmd)->heredoc = true;
		p++;
		while (pipes[p] && pipes[p] == ' ')
			p++;
		start = p;
		while (pipes[p] && pipes[p] != ' ')
			p++;
		(*cmd)->delimiter[del] = ft_substr(pipes, start, p - start);
		*i = p;	
	}
	return (0);
}

int	heredoc_checker(char *pipes, t_cmd **cmd)
{
	int i;
	int	del;
	
	del = 0;
	i = 0;
	if (heredoc_memory_allocer(pipes, cmd) == -1)
		return (-1);
	while (pipes[i])
	{
		if (pipes[i] && pipes[i] == '<')
		{
			i++;
			if (pipes[i] && pipes[i] == '<')
			{
				heredoc_copyer(pipes, cmd, &i, del);
				if (del < (*cmd)->nb_del)
					del++;
			}
		}
		else
			i++;
	}
	return (0);
}

/*Cette fonction vas recuperer chaque redirection et les ranger dans le tableau des redirecteurs*/
int	redirecter(char *pipes, t_cmd **cmd)
{
	int	i;
	int	len;
	int start;
	int	x;

	i = 0;
	len = 0;
	x = 0;
	//printf ("Nb of redirecters : %d\n", len);
	len = redirect_counter(pipes);
	(*cmd)->nb_red = len;
	if (len > 0)
		(*cmd)->redirecter = malloc(sizeof(char*)* len);
	len = 0;
	while (pipes[i])
	{
		if (pipes[i] == '<' || pipes[i] == '>')
		{
			if (pipes[i] == '<' && pipes[i + 1] == '<')
			{
				i += 2;
				while (pipes[i] && pipes[i] == ' ')
					i++;
				while (pipes[i] && pipes[i] != ' ' && pipes[i] != '<' && pipes[i] != '>')
					i++;
			}
			else
			{
				start = i;
				i++;
				len++;
				if (pipes[i] == '<' || pipes[i] == '>')
				{
					i++;
					len++;
				}
				while (pipes[i] && pipes[i] == ' ')
				{
					i++;
					len++;
				}
				while (pipes[i] && pipes[i] != ' ' && pipes[i] != '<' && pipes[i] != '>')
				{
					i++;
					len++;
				}
				(*cmd)->redirecter[x] = ft_substr(pipes, start, len);
				x++;
				len = 0;	
			}
		}
		else
			i++;
	}
	return (0);
}

/*Cette fonction vas calculer la taille que fera la commande sans les redirecteurs afin d'allouer la taille finale a la ligne de commande*/
int	len_calculator(char	*pipes)
{
	int	i;
	int	x;
	
	i = 0;
	x = 0;
	while (pipes[i])
	{
		while ((pipes[i]) && (pipes[i] != '<' && pipes[i] != '>'))
		{
			x++;
			i++;
		}
		if ((pipes[i]) && (pipes[i] == '<' || pipes[i] == '>'))
		{
			i++;
			if (pipes[i] == '<' || pipes[i] == '>')
				i++;
			while (pipes[i] && pipes[i] == ' ')
				i++;
			while (pipes[i] && pipes[i] != ' ' && pipes[i] != '<' && pipes[i] != '>')
				i++;
		}
	}
	return (x);
}

/*Cette fonction vas virer les redirecteurs afin d'avoir a traiter une ligne propre avec simplement la commande et les arguments*/
char	*redirect_deleter(char	*pipes)
{
	int		i;
	int		len;
	char	*tmp;

	tmp = NULL;
	i = 0;
	len = len_calculator(pipes);
	tmp = malloc(sizeof(char)* len + 1);
	//printf("len:%d\ni:%d\n", len, i);
	len = 0;
	while (pipes[i])
	{
		while (pipes[i] && pipes[i] != '<' && pipes[i] != '>')
		{
			if (pipes[i] == ' ' && tmp[len - 1] == ' ')
				i++;
			else
			{
				tmp[len] = pipes[i];
				len++;
				i++;
			}
		}
		if ((pipes[i]) && (pipes[i] == '<' || pipes[i] == '>'))
		{
			i++;
			if (pipes[i] == '<' || pipes[i] == '>')
				i++;
			while (pipes[i] && pipes[i] == ' ')
				i++;
			while (pipes[i] && pipes[i] != ' ' && pipes[i] != '<' && pipes[i] != '>')
				i++;
		}
	}
	//printf("LEN PIPES = %d\n", len);
	//printf("len:%d\ni:%d\n", len, i);
	//printf("tmp[i]->|%c|\n", tmp[len - 1]);
	tmp[len] = '\0';
	//printf("TMP = |%s|\n", tmp);
	free(pipes);
	return(tmp);
}

void	printf_title()
{
	printf("\033[93m");
	printf("		           _       _     _          _ _ \n");
	usleep(150000);
	printf("\033[33m");
	printf("		          (_)     (_)   | |        | | |\n");
	usleep(150000);
	printf("\033[33;2m");
	printf("		 _ __ ___  _ _ __  _ ___| |__   ___| | |\n");
	usleep(150000);
	printf("\033[38;5;220m");
	printf("		| '_ ` _ \\| | '_ \\| / __| '_ \\ / _ \\ | |\n");
	usleep(150000);
	printf("\033[38;5;228m");
	printf("		| | | | | | | | | | \\__ \\ | | |  __/ | |\n");
	usleep(150000);
	printf("\033[38;5;229m");
	printf("		|_| |_| |_|_|_| |_|_|___/_| |_|\\___|_|_|\n");
	usleep(150000);
	printf("\033[38;5;230m");
	printf("		                                        \n");
	usleep(150000);
	printf("\033[0m");
}

char	**env_copyer(char **envp, t_var *var)
{
	int 	i;
	int		len;
	char	**mini_env;

	len = 0;
	i = 0;
	while (envp[len])
		len++;
	mini_env = malloc(sizeof(char *)* len);
	if (!mini_env)
		return (NULL);
	var->len_env = len;
	while (i < len)
	{
		mini_env[i] = ft_strdup(envp[i]);
		i++;
	}
	return (mini_env);
}

void	expand_initializer(t_expand **var)
{
	(*var) = malloc(sizeof(t_expand));
	(*var)->name_start = 0;
	(*var)->name_end = 0;
	(*var)->name_len = 0;
	(*var)->name = NULL;
	(*var)->value = NULL;
	(*var)->value_len = 0;
}

char *dolls_expander(char *rl) 
{
	t_expand	*var;
	char		*output;
	int			p;
	int			i;
	
	i = 0;
	if (rl)
		output = ft_strdup(rl);
	if (!output)
		return (NULL);
	while (output[i])
	{
		if (output[i] == '$')
		{
			expand_initializer(&var);
			var->name_start = i + 1;
			var->name_end = var->name_start;
			while (output[var->name_end] && output[var->name_end] != ' ' \
			&& output[var->name_end] != '$' && output[var->name_end] != 34)
				var->name_end++;
			printf("startname = %c\nendname = %c\n", output[var->name_start], output[var->name_end]);
			var->name_len = var->name_end - var->name_start;
			var->name = ft_substr(output, var->name_start, var->name_len);
			var->value = getenv(var->name);
			if (var->value)
				var->value_len = ft_strlen(var->value);
			rl = malloc(sizeof(char)* var->value_len + i + 1);
			if (!rl)
				printf("ERROR MALLOC EXPAND\n\n\n\n");
			i = 0;
			while (i < (var->name_start - 1))
			{
				rl[i] = output[i];
				i++;
			}
			p = 0;
			if(var->value)
			{
				while (var->value[p])
				{
					rl[i] = var->value[p];
					i++;
					p++;		
				}
			}
			p = var->name_end;
			//printf("p = %d\nlen output = %ld", p, ft_strlen(output));
			while (output[p])
			{
				rl[i] = output[p];
				i++;
				p++;
			}
			rl[i] = '\0';
			output = ft_strdup(rl);
			printf("LEN OUTPUT |%ld|\nLEN RL |%ld|", ft_strlen(output), ft_strlen(rl));
			//printf ("\033[31mrl : |%s|\033[0m\n]", rl);
			//printf ("\033[31moutput : |%s|\033[0m\n]", output);
			//printf("\033[38;5;220mVarName : %s\033[0m\n", var->name);
			//printf("\033[38;5;220mVarValue : %s\033[0m\n", var->value);
			i = 0;
		}
		else
			i++;
	}
	return (free(rl), output);	
}

int	launch_exec(t_cmd *lst, char **envp)
{
    t_struct   	*var;
    int        	i;

	var = malloc(sizeof(t_struct));
	var->pipe_fd[0] = 0;
	var->pipe_fd[1] = 0;
	var->pipe_fd[2] = 0;
	var->save_pipe = 0;
	
    i = 0;
    int len_lst = ft_lstlen(lst);
	printf("nb of pipe (node) in list : %d\n", len_lst);
	printf("lst = %p\n", lst);
    while (lst)
    {
		printf("HERE\n");
        i++;
        if (lst->heredoc > 0)        //1. Heredoc check
            printf("ft_heredoc() a faire\n");
        lst->open = 0;
        if (lst->redirecter)        //2. redirecter check
            ft_redirecter(lst);

        if (lst->next != NULL)        //3. Pipe check ne peut etre fait si 3 ou plus de cmd car il va refaire un pipe et erase lancien alors aue pour 2 cmd il fait qun pipe 
        {
            if (pipe(var->pipe_fd) == -1){
                perror("pipe failed");
                exit(EXIT_FAILURE);
            }
        }

        ft_check_access(lst, envp);    //4 Cmd check

        if (i == 1 && lst->open == 0){            //5. exec (cmd_first) | cmd_middle... | cmd_last
            printf("go exec first cmd\n\n");
            ft_first_fork(lst, &var, envp);
            close(var->pipe_fd[1]);// je close lecriture pour pour pas que la lecture attendent indefinement.
            var->save_pipe = var->pipe_fd[0]; //je save la lecture pour le next car je vais re pipe pour avoir un nouveau canal 
        }

        else if (i < len_lst && lst->open == 0){//6. exec cmd_first | (cmd_middle...) | cmd_last
            printf("go exec middle cmd\n\n");
            ft_middle_fork(lst, &var, envp);
            close(var->pipe_fd[1]);
            var->save_pipe = var->pipe_fd[0];
        }

        else if (i == len_lst && lst->open == 0){//7. exec  exec cmd_first | cmd_middle... | (cmd_last)
            printf("go exec last cmd\n\n");
            
            ft_last_fork(lst, &var, envp);
            close(var->pipe_fd[0]);
            }

        //ft_free_token(lst);
        ft_close(lst);
        lst = lst->next;
    }
    return (0);
}

int main(int argc, char **argv, char **envp)
{
	char	*rl;
	char	*pwd;
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
		pwd = getcwd(NULL, 0);	//je recupere le chemin d'acces pour l'afficher tel fish
		//var->mini_env = env_copyer(envp, var); // je recupere et copie lenvironnement dans un autre tableau
		printf ("\033[90m%s\033[0m", pwd);	//je l'affiche
		rl = readline("\e[33m$> \e[37m");	//je recupere la ligne en entree dans une boucle infini afin de l'attendre
		printf("==========RL : %s\n", rl);
		if (syntaxe_error(rl) == 0)	//je check les erreurs de syntaxes et lance le programme seulement si tout est OK
		{
			printf("RL = \e[31m%s\n\e[0m", rl);
			negative_checker(rl);		//je check toutes mes cotes et passe en negatif tout ce qui ne vas pas devoir etre interprete
			printf("RL after cotes checker = \e[31m%s\n\e[0m", rl);
			rl = dolls_expander(rl);
			printf("RL  after expander = \e[31m%s\n\e[0m", rl);
			pipes = ft_split(rl, '|');	//je separe chaque commande grace au pipe
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
				free(pipes[i]);
				if (stock_input(input, &tmp) == 0)	//je vais recuperer le nom de la commande et les arguments dans input et les ranger dans la structure tmp
				{
					i++;	// la commande a ete traitee, je passe a la prochaine (si il y en a une)
					ft_lstadd_back_minishell(&cmd, tmp);	//j'ajoute le noeud tmp ou tout a ete ranger a ma liste chainee 
				}
			}
			i = 0;
			ft_printf_struct(cmd);
			printf("\033[38;5;220mLancement de Launch_exec...\n\033[0m");
			printf("cmd = %p\n", cmd);
			if (launch_exec(cmd, envp) == -1)
				printf ("Error exec\n");
			printf("--------------------------------------------------------\n");
			ft_lstclear(&cmd);
			free(pipes);
			free(pwd);
		}
	}
}
