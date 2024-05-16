/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gavairon <gavairon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:39:44 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/16 01:28:12 by gavairon         ###   ########.fr       */
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
	if (mini_env->value)
		len = (ft_strlen(mini_env->name) + ft_strlen(mini_env->value) + 2);
	else
		len = (ft_strlen(mini_env->name) + 2);
	line = malloc(sizeof(char) * len);
	if (!line)
		return (NULL);
	len = 0;
	while (mini_env->name[len])
	{
		line[len] = mini_env->name[len];
		len++;
	}
	line[len++] = '=';
	if (mini_env->value)
	{
		while (mini_env->value[i])
			line[len++] = mini_env->value[i++];
	}
	line[len] = '\0';
	return (line);
}

int	ft_envsize(t_env *mini_env)
{
	t_env	*start;
	int		i;

	start = mini_env;
	i = 0;
	while (mini_env)
	{
		i++;
		mini_env = mini_env->next;
	}
	mini_env = start;
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
	tab = ft_calloc(len, sizeof(char *) + 1);
	if (!tab)
		return (NULL);
	while (tmp)
	{
		tab[i++] = line_extractor(tmp);
		tmp = tmp->next;
	}
	return (tab);
}

char	*line_extractor_cote(t_env *mini_env)
{
	char	*line;
	int		len;
	int		i;

	i = 0;
	len = 0;
	line = NULL;
	if (mini_env->value)
		len = (ft_strlen(mini_env->name) + ft_strlen(mini_env->value) + 4);
	else
		len = (ft_strlen(mini_env->name) + 4);
	line = malloc(sizeof(char) * len);
	if (!line)
		return (NULL);
	len = 0;
	while (mini_env->name[len])
	{
		line[len] = mini_env->name[len];
		len++;
	}
	if (mini_env->value)
	{
		line[len++] = '=';
		line[len++] = '"';
		while (mini_env->value[i])
			line[len++] = mini_env->value[i++];
		line[len++] = '"';
	}
	line[len] = '\0';
	return (line);
}

char	**ft_list_to_tab_cote(t_env *mini_env)
{
	char	**tab;
	t_env	*tmp;
	int		len;
	int		i;

	i = 0;
	len = 0;
	tab = NULL;
	tmp = mini_env;
	len = ft_envsize(mini_env);
	tab = ft_calloc(len, sizeof(char *) + 1);
	if (!tab)
		return (NULL);
	while (tmp)
	{
		tab[i++] = line_extractor_cote(tmp);
		tmp = tmp->next;
	}
	return (tab);
}
