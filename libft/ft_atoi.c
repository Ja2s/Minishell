/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:16:20 by jgavairo          #+#    #+#             */
/*   Updated: 2023/11/23 12:00:32 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	size_t	i;
	int		n;
	long	nbr;

	nbr = 0;
	n = 1;
	i = 0;
	while ((nptr[i] == 32) || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			n *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nbr *= 10;
		if ((nbr < 0 && n == 1) || (nbr >= 3689348814741910323))
			return (-1);
		if ((nbr < 0 && n == -1) || (nbr >= 3689348814741910323))
			return (0);
		nbr = nbr + nptr[i++] - '0';
	}
	return (nbr * n);
}
