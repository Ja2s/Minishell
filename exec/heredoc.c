/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:49:33 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/14 17:09:19 by rasamad          ###   ########.fr       */
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
		if (!tab)
			return (NULL);
		tab[i] = ft_strdup(rl);
		if (!tab)
			return (NULL);
		tab[i + 1] = NULL;
		return (tab);
	}
	tab = malloc(sizeof(char*) * (i + 2));
	if (!tab)
		return (NULL);
	i = 0;
	while (lst->heredoc_content[i])
	{
		tab[i] = ft_strdup(lst->heredoc_content[i]);
		if (!tab)
			return (NULL);
		i++;
	}
	tab[i] = ft_strdup(rl);
	if (!tab)
		return (NULL);
	tab[i + 1] = NULL;
	return (tab);
}

int ft_heredoc(t_data *data) 
{
    int i = 0;  // Indice du nombre de Heredocs
    char *line;
	t_cmd	*lst;

	lst = data->cmd;
    lst->heredoc_content = NULL;
	while (lst)
	{
		while (i < lst->nb_del && lst->heredoc == true) 
		{
			line = readline(">");
			while (ft_strcmp(line, lst->delimiter[i]) != 0)
			{
				if (lst->expand_heredoc == 1)
					line = dolls_expander(line, data->mini_env, data);
				lst->heredoc_content = ft_realloc(line, lst);
				if (!lst->heredoc_content)
					return(exit_status(data, 1, "Malloc error from [ft_heredoc]\n"), -1);
				free(line);  // Libère la mémoire allouée par readline
				line = readline(">");
			}
			i++;
			if (i  < lst->nb_del)
			{
				printf("end heredoc\n");
				//ft_free_heredoc(lst)
				lst->heredoc_content = NULL; //delete old heredoc
			}
		}
		i = 0;
		lst = lst->next;
	}
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
		while (lst->heredoc_content && lst->heredoc_content[j])
		{
			printf("%d|%s|\n", j, lst->heredoc_content[j]);
			j++;
		}
	j = 0;
	lst = lst->next;
	}
}
