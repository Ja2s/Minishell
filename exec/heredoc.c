/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:49:33 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/03 17:47:34 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_all_heredoc(t_cmd *lst)
{
	while (lst)
	{
		if (lst->heredoc_content)
		{
			printf("ciao %s", lst->heredoc_content);
			free(lst->heredoc_content);
			lst->heredoc_content = NULL;
		}
		lst = lst->next;
	}
}

void    ft_free_heredoc(t_cmd *lst)
{
    int    i;

    i = 0;
    printf("end heredoc\n");
    if (lst->heredoc_content)
    {
        free(lst->heredoc_content);
        lst->heredoc_content = NULL;
    }
}

void handle_sigint_heredoc(int sig) 
{
    (void)sig;
    g_sig = 1;
	printf("^C");
    rl_done = 1; // Interrompt readline proprement
}

int	rl_hook_function()
{
	signal(SIGINT, handle_sigint_heredoc);
	return (0);
}

int ft_heredoc(t_data *data) 
{
    int i = 0;  // Indice du nombre de Heredocs
    char *line;
	t_cmd	*lst;
	char *tmp;

	lst = data->cmd;
    lst->heredoc_content = NULL;

	lst->del_one = 0;
	rl_callback_handler_install("", NULL);
	rl_event_hook = rl_hook_function;
	while (lst)
	{
		while (i < lst->nb_del && lst->heredoc == true) 
		{
			line = readline(">");
			if (g_sig) {
				exit_status(data, 130, "");
				free(line);
				rl_callback_handler_remove(); // Retirer le gestionnaire de readline
				//signal(SIGINT, handle_sigint_main); // Restaure le gestionnaire de signal SIGINT principal
				return -1; // Quitter la fonction si un signal SIGINT a été reçu
			}
			if (line == NULL)
			{
				printf("bash: warning: here-document delimited by end-of-file (wanted `%s')\n", lst->delimiter[i]);
				line = ft_strdup(lst->delimiter[i]);
			}
			if (ft_strcmp(line, lst->delimiter[i]) == 0)
			{
				free(line);
				lst->heredoc_content = malloc(1 * sizeof(char));
				lst->heredoc_content[0] = '\n';
				lst->del_one = 1;
			}
			else
			{
				lst->del_one = 0;
				while (ft_strcmp(line, lst->delimiter[i]) != 0)
				{
					if (lst->expand_heredoc == 1)
						line = dolls_expander(line, data);
					tmp = ft_strjoin(lst->heredoc_content, line);
					free(lst->heredoc_content);
					lst->heredoc_content = tmp;
					tmp = ft_strjoin(lst->heredoc_content, "\n");
					free(lst->heredoc_content);
					lst->heredoc_content = tmp;
					free(line);  // Libère la mémoire allouée par readline
					if (!lst->heredoc_content)
						return(exit_status(data, 1, "Malloc error from [ft_heredoc]\n"), -1);
					line = readline(">");
					if (line == NULL)
					{
						printf("bash: warning: here-document delimited by end-of-file (wanted `%s')\n", lst->delimiter[i]);
						line = ft_strdup(lst->delimiter[i]);
					}
					if (g_sig)
					{
						exit_status(data, 130, "");
						free(line);
						rl_callback_handler_remove(); // Retirer le gestionnaire de readline
						//signal(SIGINT, handle_sigint_main); // Restaure le gestionnaire de signal SIGINT principal
						return (-1); // Quitter la fonction si un signal SIGINT a été reçu
					}
				}
				free(line);  // Libère la mémoire allouée par readline	
			}
			i++;
			if (i  < lst->nb_del)
				ft_free_heredoc(lst);
		}
		i = 0;
		lst = lst->next;
	}
	rl_callback_handler_remove(); // Retirer le gestionnaire de readline
	//signal(SIGINT, handle_sigint_main); // Restaure le gestionnaire de signal SIGINT principal
	return (0);
}

void	ft_display_heredoc(t_cmd *lst)
{
	int i;
	int j;

	i = 1;
	j = 0;
	while (lst != NULL)
	{
		printf("\nheredoc %d :\n", i);
		i++;
		printf("|%s|\n", lst->heredoc_content);
		lst = lst->next;
	}
}
