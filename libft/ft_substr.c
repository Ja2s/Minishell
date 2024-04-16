/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:27:57 by jgavairo          #+#    #+#             */
/*   Updated: 2023/11/17 14:58:42 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	str_len;

	str = NULL;
	str_len = ft_strlen(s);
	i = 0;
	if (start >= str_len)
	{
		str = ft_calloc(1, sizeof(char));
		return (str);
	}
	if (start + len > str_len)
		str = malloc ((str_len - start + 1) *(sizeof(char)));
	else
		str = malloc((len + 1) *(sizeof(char)));
	if (!str)
		return (NULL);
	while (i < (str_len - start) && i < len)
	{
		str[i] = s[i + start];
		i++;
	}
	str[i] = '\0';
	return (str);
}
