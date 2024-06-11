/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:49:33 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/11 11:24:16 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_all_heredoc(t_cmd *lst)
{
	while (lst)
	{
		if (lst->heredoc_content)
		{
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
	rl_event_hook = rl_hook_function;//CRi
	while (lst)
	{
		while (i < lst->nb_del && lst->heredoc == true) 
		{
			line = readline(">");
			if (g_sig)
						return (exit_status(data, 130, ""), free(line), -1); // Quitter la fonction si un signal SIGINT a été reçu
			if (line == NULL)
			{
				printf("bash: warning: here-document delimited by end-of-file (wanted `%s')\n", lst->delimiter[i]);
				line = ft_strdup(lst->delimiter[i]);
				if (!line)
					return (exit_status(data, 1, "malloc error from [ft_strdup] heredoc.c\n"), -1);
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
					free(line);  // Libère la mémoire allouée par readline
					if (!tmp)
						return (exit_status(data, 1, "malloc error from [ft_strjoin] heredoc.c\n"), -1);
					free(lst->heredoc_content);
					lst->heredoc_content = tmp;
					tmp = ft_strjoin(lst->heredoc_content, "\n");
					if (!tmp)
						return (exit_status(data, 1, "malloc error from [ft_strjoin 2] heredoc.c\n"), -1);
					free(lst->heredoc_content);
					lst->heredoc_content = tmp;
					line = readline(">");
					if (line == NULL)
					{
						printf("bash: warning: here-document delimited by end-of-file (wanted `%s')\n", lst->delimiter[i]);
						line = ft_strdup(lst->delimiter[i]);
						if (!line)
							return (exit_status(data, 1, "malloc error from [ft_strdup 2] heredoc.c\n"), -1);
					}
					if (g_sig)
						return (exit_status(data, 130, ""), free(line), -1); // Quitter la fonction si un signal SIGINT a été reçu
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
	return (0);
}
