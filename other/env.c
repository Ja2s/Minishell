/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:39:44 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/03 16:41:58 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*line_extractor(t_env *mini_env)
{
	char	*line;
	int		len;
	int		i;

	i = 0;
	len = 0;
	line = NULL;
	len = (ft_strlen(mini_env->name) + ft_strlen(mini_env->value) + 2);
	line = malloc(sizeof(char)* len);
	if (!line)
		return (NULL);
	len = 0;
	while (mini_env->name[len])
	{
		line[len] = mini_env->name[len];
		len++;
	}
	line[len++] = '=';
	while (mini_env->value[i])
		line[len++] = mini_env->value[i++];
	line[len] = '\0';
	return (line);
}

int	ft_envsize(t_env *mini_env)
{
	int	i;

	i = 0;
	while(mini_env)
	{
		i++;
		mini_env = mini_env->next;
	}
	return (i);
}

char	**ft_list_to_tab(t_env *mini_env)
{
	char	**tab;
	t_env	*tmp;
	int		len;
	int		i;

	i = 0;
	tab = NULL;
	tmp = mini_env;
	len = ft_envsize(mini_env);
	tab = ft_calloc(len, sizeof(char*) + 1);
	if(!tab)
		return (NULL);
	while (tmp)
	{
		tab[i++] = line_extractor(tmp);
		tmp = tmp->next;
	}
	return (tab);
}
