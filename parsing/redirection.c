/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:27:51 by jgavairo          #+#    #+#             */
/*   Updated: 2024/04/29 13:33:55 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*Cette fonction vas servir a checker le nombre de redirecteurs present dans la commande pour pouvoir allouer la bonne taille au tableau de redirection*/
int	redirect_counter(char *pipes)
{
	int	i;
	int	len;

	len = 0;
	i = 0;
	while (pipes[i])
	{
		if (pipes[i] == '<' || pipes[i] == '>')
		{
			if (pipes[i] == '<' && pipes[i + 1] == '<')
				i = i + 2;
			else
			{
				len++;
				i++;
				if (pipes[i] == '<' || pipes[i] == '>')
					i++;
			}
		}
		else
			i++;
	}
	return (len);
}

/*Cette fonction vas recuperer chaque redirection et les ranger dans le tableau des redirecteurs*/
int	redirecter(char *pipes, t_cmd **cmd)
{
	int	i;
	int	len;
	int start;
	int	x;

	i = 0;
	len = 0;
	x = 0;
	//printf ("Nb of redirecters : %d\n", len);
	len = redirect_counter(pipes);
	(*cmd)->nb_red = len;
	if (len > 0)
	{	
		(*cmd)->redirecter = malloc(sizeof(char*) * (len + 1));
		printf("____________________________\nLEN = %d\n______________________\n", len);
		len = 0;
		while (pipes[i])
		{
			if (pipes[i] == '<' || pipes[i] == '>')
			{
				if (pipes[i] == '<' && pipes[i + 1] == '<')
				{
					i += 2;
					while (pipes[i] && pipes[i] == ' ')
						i++;
					while (pipes[i] && pipes[i] != ' ' && pipes[i] != '<' && pipes[i] != '>')
						i++;
				}
				else
				{
					start = i;
					i++;
					len++;
					if (pipes[i] == '<' || pipes[i] == '>')
					{
						i++;
						len++;
					}
					while (pipes[i] && pipes[i] == ' ')
					{
						i++;
						len++;
					}
					while (pipes[i] && pipes[i] != ' ' && pipes[i] != '<' && pipes[i] != '>')
					{
						i++;
						len++;
					}
					(*cmd)->redirecter[x] = ft_substr(pipes, start, len);
					x++;
					len = 0;	
				}
			}
			else
				i++;
		}
		printf("____________________________\nX = %d\n______________________\n", x);
		(*cmd)->redirecter[x] = NULL;
	}
	return (0);
}
/*Cette fonction vas calculer la taille que fera la commande sans les redirecteurs afin d'allouer la taille finale a la ligne de commande*/
int	len_calculator(char	*pipes)
{
	int	i;
	int	x;
	
	i = 0;
	x = 0;
	while (pipes[i])
	{
		while ((pipes[i]) && (pipes[i] != '<' && pipes[i] != '>'))
		{
			x++;
			i++;
		}
		if ((pipes[i]) && (pipes[i] == '<' || pipes[i] == '>'))
		{
			i++;
			if (pipes[i] == '<' || pipes[i] == '>')
				i++;
			while (pipes[i] && pipes[i] == ' ')
				i++;
			while (pipes[i] && pipes[i] != ' ' && pipes[i] != '<' && pipes[i] != '>')
				i++;
		}
	}
	return (x);
}

/*Cette fonction vas virer les redirecteurs afin d'avoir a traiter une ligne propre avec simplement la commande et les arguments*/
char	*redirect_deleter(char	*pipes)
{
	int		i;
	int		len;
	char	*tmp;

	tmp = NULL;
	i = 0;
	len = 0;
	len = len_calculator(pipes);
	tmp = ft_calloc((len + 1), sizeof(char));
	len = 0;
	while (pipes[i])
	{
		while (pipes[i] && pipes[i] != '<' && pipes[i] != '>')
		{
			if (pipes[i] == ' ' && tmp[len/* - 1*/] == ' ')
				i++;
			else
			{
				tmp[len] = pipes[i];
				len++;
				i++;
			}
		}
		if ((pipes[i]) && (pipes[i] == '<' || pipes[i] == '>'))
		{
			i++;
			if (pipes[i] == '<' || pipes[i] == '>')
				i++;
			while (pipes[i] && pipes[i] == ' ')
				i++;
			while (pipes[i] && pipes[i] != ' ' && pipes[i] != '<' && pipes[i] != '>')
				i++;
		}
	}
	free(pipes);
	return(tmp);
}
