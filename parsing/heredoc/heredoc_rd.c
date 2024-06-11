/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_rd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:12:41 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/11 11:14:50 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*heredoc_w_cote(char *src)
{
	char	*dest;
	char	*tmp;
	int		i;
	int		p;

	tmp = NULL;
	dest = NULL;
	p = 0;
	i = 0;
	dest = ft_calloc((ft_strlen(src) - 1), sizeof (char));
	if (!dest)
		return (NULL);
	tmp = ft_strdup(src);
	if (!tmp)
		return (NULL);
	while (tmp[i])
	{
		if (tmp[i] == 34 || tmp[i] == 39)
			i++;
		else
			dest[p++] = tmp[i++];
	}
	free(tmp);
	free(src);
	return (dest);
}
