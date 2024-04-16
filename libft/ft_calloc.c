/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 11:30:57 by jgavairo          #+#    #+#             */
/*   Updated: 2023/11/22 18:22:40 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*ptr;
	size_t			t_size;

	t_size = (nmemb * size);
	if (nmemb == 0 || size == 0)
	{
		ptr = malloc(0);
		if (!ptr)
			return (NULL);
	}
	else if (t_size / nmemb != size)
		return (NULL);
	else
	{
		ptr = malloc(t_size);
		if (!ptr)
			return (NULL);
		ft_memset(ptr, '\0', t_size);
	}
	return (ptr);
}
