/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 00:41:38 by jgavairo          #+#    #+#             */
/*   Updated: 2023/11/20 18:40:00 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	lendst;
	size_t	lensrc;

	i = 0;
	lendst = 0;
	lensrc = 0;
	while (dst[lendst] && lendst < size)
		lendst++;
	while (src[lensrc])
		lensrc++;
	while (src[i] && lendst + i + 1 < size)
	{
		dst[lendst + i] = src[i];
		i++;
	}
	if (lendst < size)
		dst[lendst + i] = '\0';
	return (lendst + lensrc);
}
