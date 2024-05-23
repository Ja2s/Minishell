/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxe_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gavairon <gavairon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:07:14 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/23 22:49:03 by gavairon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	double_cote_checker(char *rl, bool *cot, int i)
{
	int	p;

	*cot = false;
	p = i;
	while (rl[p] && *cot == false)
	{
		p++;
		if (rl[p] == 34)
			*cot = true;
	}
	if (*cot == false)
		return (-1);
	return (p);
}

int	simple_cote_checker(char *rl, bool *cot, int *i)
{
	int	p;

	*cot = false;
	p = *i;
	while (rl[p] && *cot == false)
	{
		p++;
		if (rl[p] == 39)
			*cot = true;
	}
	if (*cot == false)
		return (-1);
	*i = p;
	return (0);
}

int	skip_cote(char *rl, int i, int choice)
{
	if (choice == 2)
	{
		i++;
		while (rl[i] != 34)
			i++;
	}
	else if (choice == 1)
	{
		i++;
		while (rl[i] != 39)
			i++;
	}
	return (i);
}

int	double_pipe_checker(char *rl)
{
	int	i;

	i = 0;
	while (rl[i])
	{
		if (rl[i] == 34)
			i = skip_cote(rl, i, 2);
		else if (rl[i] == 39)
			i = skip_cote(rl, i, 1);
		else if (rl[i] == '|' && rl[i + 1] == '|')
			return (-1);
		else if (rl[i] == '|')
		{
			i++;
			while (ft_isspace(rl[i]) == 1)
				i++;
			if (rl[i] == '|' || rl[i] == '\0')
				return (-1);
		}
		i++;
	}
	return (0);
}

int	cote_checker(char *rl)
{
	int		i;
	bool	cot;

	cot = true;
	i = 0;
	while (rl[i])
	{
		if (rl[i] == 34)
		{
			i = double_cote_checker(rl, &cot, i);
			if (i == -1)
				return (-1);
		}
		if (rl[i] == 39)
		{
			if (simple_cote_checker(rl, &cot, &i) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}

int	syntaxe_error(t_data *data, char *rl)
{
	if (cote_checker(rl) == -1)
		return (exit_status(data, 2, \
		"\033[31mSyntaxe error [cotes]\n\033[0m"), -1);
	if (rafters_checker(rl) == -1)
		return (exit_status(data, 2, \
		"\033[31mSyntaxe error [rafters]\n\033[0m"), -1);
	if (double_pipe_checker(rl) == -1)
		return (exit_status(data, 2, \
		"\033[31mSyntaxe error [pipes]\n\033[0m"), -1);
	if (starter_pipe(rl) == -1)
		return (exit_status(data, 2, \
		"\033[31mSyntaxe error [pipes]\n\033[0m"), -1);
	return (0);
}
