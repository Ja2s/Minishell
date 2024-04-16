/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 21:10:49 by jgavairo          #+#    #+#             */
/*   Updated: 2023/11/22 19:38:09 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count_words(char const *s, char c);
static void		*ft_free(char **tab);

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	start;
	size_t	end;
	char	**tab;

	start = 0;
	i = 0;
	tab = ft_calloc((ft_count_words(s, c) + 1), sizeof(char *));
	if (!tab)
		return (NULL);
	while (i < ft_count_words(s, c))
	{
		while (s[start] == c && s[start] != '\0')
			start++;
		end = start;
		while (s[end] != c && s[end] != '\0')
			end++;
		tab[i] = ft_substr(s, start, end - start);
		if (!tab[i])
			return (ft_free(tab));
		start = end;
		i++;
	}
	return (tab);
}

static size_t	ft_count_words(char const *s, char c)
{
	size_t	word;
	size_t	i;
	size_t	len;

	len = ft_strlen(s);
	word = 0;
	i = 0;
	while (s[i] == c && i < len)
		i++;
	if (s[i] != c && i < len)
		word++;
	while (i < len)
	{
		while (s[i] != c && i < len)
			i++;
		while (s[i] == c && i < len)
		{
			i++;
			if (s[i] != c && i < len)
				word++;
		}
	}
	return (word);
}

static void	*ft_free(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i] != NULL)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}
