/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxe_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:07:14 by jgavairo          #+#    #+#             */
/*   Updated: 2024/04/29 11:50:28 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	double_cote_checker(char *rl, bool *cot, size_t *i)
{
	size_t	p;

	*cot = false;
	p = *i;
	while (rl[p] && *cot == false)
	{
		p++;
		if (rl[p] == 34)
			*cot = true;
	}
	if (*cot == false)
		return (-1);
	*i = p;
	return (0);
}

int	simple_cote_checker(char *rl, bool *cot, size_t *i)
{
	size_t	p;

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

/*Cette fonction bas checker les erreurs de syntaxes lie aux chevrons (exemple "<< <", ">>>" ou avec aucun nom de fichier)*/
int	rafters_checker(char *rl)
{
	int	i;

	i = 0;
	while (rl[i])
	{
		if (rl[i] == '<' || rl[i] == '>')
		{
			i++;
			if (rl[i] == '<' || rl[i] == '>')
				i++;
			while (rl[i] == ' ')
				i++;
			if (rl[i] == '\0' || rl[i] == '<' || rl[i] == '>' || rl[i] == '|')
				return (-1);
		}
		else
			i++;
	}
	return (0);
}

/*Cette fonction vas checker qu'aucun double pipe n'exite dans lentree, si c'est le cas, retourne une erreur de syntaxe*/
int	double_pipe_checker(char *rl)
{
	size_t	i;

	i = 0;
	while (rl[i])
	{
		if (rl[i] == '|' && rl[i + 1] == '|')
			return (-1);
		i++;
	}
	return (0);
}

/*Cette fonction vas checker que toutes les cotes sont bien et si ce n'est pas le cas, provoque une erreur de syntaxe*/
int	cote_checker(char *rl)
{
	size_t	i;
	bool	cot;

	cot = true;
	i = 0;
	while (rl[i])
	{
		if (rl[i] == 34)
		{
			if (double_cote_checker(rl, &cot, &i) == -1)
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

int	syntaxe_error(char	*rl)
{
	if (rafters_checker(rl) == -1)
		return (printf("\033[31mSyntaxe error\n\033[0m"), -1);
	if (cote_checker(rl) == -1)
		return (printf("\033[31mSyntaxe error\n\033[0m"), -1);
	if (double_pipe_checker(rl) == -1)
		return (printf("\033[31mSyntaxe error\n\033[0m"), -1);
	return (0);
}