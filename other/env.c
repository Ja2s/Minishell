/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gavairon <gavairon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:39:44 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/23 00:28:40 by gavairon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_len(t_env *mini_env, int choice)
{
	int	len;
	if (choice == 1)
	{
		if (mini_env->value)
			len = (ft_strlen(mini_env->name) + ft_strlen(mini_env->value) + 2);
		else
			len = (ft_strlen(mini_env->name) + 2);
	}
	if (choice == 2)
	{
		if (mini_env->value)
			len = (ft_strlen(mini_env->name) + ft_strlen(mini_env->value) + 4);
		else
			len = (ft_strlen(mini_env->name) + 4);
	}
	return (len);
}

char	*line_extractor(t_env *mini_env)
{
	char	*line;
	int		len;
	int		i;

	i = 0;
	len = check_len(mini_env, 1);
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
	t_env	*tmp;
	int		i;

	tmp = mini_env;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
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
	tab = ft_calloc(len + 1, sizeof(char *));
	//verif malloc
	if (!tab)
		return (NULL);
	while (tmp)
	{
		tab[i++] = line_extractor(tmp);
		//verif malloc
		tmp = tmp->next;
	}
	tab[i] = NULL;
	return (tab);
}

char	*line_extractor_cote(t_env *mini_env)
{
	char	*line;
	int		len;
	int		i;

	i = 0;
	len = check_len(mini_env, 2);
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
