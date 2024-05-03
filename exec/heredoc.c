/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:49:33 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/03 15:50:29 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**ft_realloc(char *rl, t_cmd *lst)
{
	size_t		i;
	char	**tab;

	tab = NULL;
	i = 0;
	while (lst->heredoc_content && lst->heredoc_content[i])
		i++;
	if (i == 0)
	{
		tab = malloc(sizeof(char *) * 2);
		tab[i] = ft_strdup(rl);
		tab[i + 1] = NULL;
		return (tab);
	}
	tab = malloc(sizeof(char*) * (i + 2));
	i = 0;
	while (lst->heredoc_content[i])
	{
		tab[i] = ft_strdup(lst->heredoc_content[i]);
		i++;
	}
	tab[i] = ft_strdup(rl);
	tab[i + 1] = NULL;
	return (tab);
}

int ft_heredoc(t_cmd *lst) 
{
    int i = 0;  // Indice du nombre de Heredocs
    char *line;

    lst->heredoc_content = NULL;
	while (lst)
	{
		while (i < lst->nb_del) 
		{
			line = readline(">");
			while (ft_strncmp(line, lst->delimiter[i], ft_strlen(line)) != 0)
			{
				lst->heredoc_content = ft_realloc(line, lst);
				free(line);  // Libère la mémoire allouée par readline
				line = readline(">");
			}
			printf("end heredoc\n");
			i++;
			if (i  < lst->nb_del)
			{
				//ft_free_heredoc(lst)
				lst->heredoc_content = NULL; //delete old heredoc
			}
		}
		i = 0;
		lst = lst->next;
	}
    return 0;
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
		while (lst->heredoc_content && lst->heredoc_content[j])
		{
			printf("%d|%s|\n", j, lst->heredoc_content[j]);
			j++;
		}
	j = 0;
	lst = lst->next;
	}
}
