/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_stocker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 12:51:59 by jgavairo          #+#    #+#             */
/*   Updated: 2024/04/29 13:47:14 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	pipes_counter(char *rl)
{
	int	i;

	i = 0;
	while (rl[i])
	{
		if (rl[i] == '|')
			return (1);
		i++;
	}
	return (0);
}

/*Cette fonction vas chercher les infos dans input, et les ranger dans le noeud de la structure*/
void	command_stocker(char **input, t_cmd **cmd)
{
	int	i;

	i = 0;
	// if (input[0][0] != '<' && input[0][0] != '>')
	// 	(*cmd)->args[i] = ft_strdup(input[i]);
	//i++;
	while (input[i])
	{
		(*cmd)->args[i] = ft_strdup(input[i]);
		i++;
	}
	(*cmd)->args[i] = NULL;
}

/*Cette fonction vas aller check le nombre d'arguments qui sont present dans la commande pour allouer la bonne taille au tableau associe*/
int	args_memory_alloc(char **input, t_cmd **cmd)
{
	int	i;

	i = 0;
	while (input[i])
		i++;
	(*cmd)->nb_args = i;
	(*cmd)->args = malloc(sizeof(char *) * (i + 1));
	if (!(*cmd)->args)
		return (-1);
	return (0);
}

/*Cette fonction vas appeler la fonction plus haute, (de base cetait plus propre et plus logique de l'avoir vu que je faisai pariel avec les flags)*/
int	memory_alloc(char **input, t_cmd **cmd)
{
	if (args_memory_alloc(input, cmd) == -1)
		return (-1);
	return (0);
}

/*Cette fonction vas donc allouer la taille necessaire et ensuite aller stocker la commande dans le noeud*/
int	stock_input(char **input, t_cmd **cmd)
{
	if (memory_alloc(input, cmd) == -1)
		return (-1);
	command_stocker(input, cmd); 
	return (0);
}