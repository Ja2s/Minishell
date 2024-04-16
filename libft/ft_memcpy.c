/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:56:22 by jgavairo          #+#    #+#             */
/*   Updated: 2023/11/21 14:20:54 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*strdest;
	const unsigned char	*strsrc;
	size_t				i;

	i = 0;
	strdest = (unsigned char *)dest;
	strsrc = (const unsigned char *)src;
	if (strdest == NULL && strsrc == NULL)
		return (NULL);
	while (n > 0)
	{
		strdest[i] = strsrc[i];
		i++;
		n--;
	}
	return (strdest);
}
