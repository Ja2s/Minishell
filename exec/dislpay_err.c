/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dislpay_err.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:19:08 by rasamad           #+#    #+#             */
/*   Updated: 2024/06/11 22:30:43 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	display_error_cmd(t_cmd *lst)
{
	ft_putstr_fd(lst->args[0], 2);
	ft_putstr_fd(": command not found", 2);
	write(2, "\n", 1);
}

void	display_perror(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	perror(" ");
}

void	display_no_such(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": No such file or directory", 2);
	write(2, "\n", 1);
}
