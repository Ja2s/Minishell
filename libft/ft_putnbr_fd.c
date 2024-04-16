/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 10:37:03 by jgavairo          #+#    #+#             */
/*   Updated: 2023/11/20 10:37:03 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n != -2147483648)
	{
		if (n == 0)
		{
			ft_putchar_fd('0', fd);
			return ;
		}
		if (n < 0 && n != -2147483648)
		{
			n *= -1;
			ft_putchar_fd('-', fd);
		}
		if (n >= 10)
			ft_putnbr_fd(n / 10, fd);
		ft_putchar_fd(n % 10 + '0', fd);
	}
	else if (n == -2147483648)
		ft_putstr_fd("-2147483648", fd);
}
