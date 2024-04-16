/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 15:02:09 by jgavairo          #+#    #+#             */
/*   Updated: 2023/11/17 15:02:09 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char			*ft_reverse(char *str);
static size_t		ft_isnegative(long nbr);
static size_t		ft_count_number(long nbr);

char	*ft_itoa(int n)
{
	size_t	i;
	size_t	negative;
	long	nbr;
	char	*result;

	nbr = n;
	i = 0;
	negative = 0;
	if (nbr == 0)
		return (ft_strdup("0"));
	negative = ft_isnegative(nbr);
	if (negative == 1)
		nbr *= -1;
	result = ft_calloc((ft_count_number(nbr) + negative + 1), (sizeof(char)));
	if (!result)
		return (NULL);
	while (nbr != 0)
	{
		result[i++] = ((nbr % 10) + '0');
		nbr /= 10;
	}
	if (negative == 1)
		result[i] = '-';
	ft_reverse(result);
	return (result);
}

static size_t	ft_isnegative(long nbr)
{
	int	negative;

	negative = 0;
	if (nbr < 0)
	{
		negative = 1;
	}
	return (negative);
}

static size_t	ft_count_number(long nbr)
{
	size_t	i;

	i = 0;
	while (nbr != 0)
	{
		nbr /= 10;
		i++;
	}
	return (i);
}

static char	*ft_reverse(char *str)
{
	size_t		i;
	size_t		len;
	char		tmp;

	i = 0;
	len = (ft_strlen(str) - 1);
	while (i < len)
	{
		tmp = str[i];
		str[i] = str[len];
		str[len] = tmp;
		i++;
		len--;
	}
	return (str);
}
