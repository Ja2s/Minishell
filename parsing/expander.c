/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:15:44 by jgavairo          #+#    #+#             */
/*   Updated: 2024/04/30 16:43:19 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**env_copyer(char **envp, t_var *var)
{
	int 	i;
	int		len;
	char	**mini_env;

	len = 0;
	i = 0;
	while (envp[len])
		len++;
	mini_env = malloc(sizeof(char *)* len);
	if (!mini_env)
		return (NULL);
	var->len_env = len;
	while (i < len)
	{
		mini_env[i] = ft_strdup(envp[i]);
		i++;
	}
	return (mini_env);
}

/*Cette fonction vas parcourir toute lentree, et passer en negatif les caracteres speciaux entre cotes, suivant ceux que nous voulons interpreter ou pas*/
void	negative_checker(char *rl)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(rl);
	while (i < len)
	{
		if (rl[i] == 34)
		{
			i++;
			while (rl[i] != 34)
			{
				if (rl[i] == ' ' || rl[i] == '<' || rl[i] == '>')
					rl[i] = rl[i] * -1;
				i++;
			}
		}
		if (rl[i] == 39)
		{
			i++;
			while (rl[i] != 39)
			{
				if (rl[i] == ' ' || rl[i] == '$' || rl[i] == '<' || rl[i] == '>' || rl[i] == '|' || rl[i] == 34)
				rl[i] = rl[i] * -1;
				i++;
			}
		}
		i++;
	}
}

void	expand_initializer(t_expand **var)
{
	(*var) = malloc(sizeof(t_expand));
	(*var)->name_start = 0;
	(*var)->name_end = 0;
	(*var)->name_len = 0;
	(*var)->name = NULL;
	(*var)->value = NULL;
	(*var)->value_len = 0;
}
void	free_expand(t_expand **var)
{
	if ((*var)->name)
		free((*var)->name);
	if ((*var)->value)
		free((*var)->value);
	if (*var)
		free(*var);
}

char *dolls_expander(char *rl) 
{
	t_expand	*var;
	char		*output;
	int			p;
	int			i;
	
	output = NULL;
	i = 0;
	if (rl)
		output = ft_strdup(rl);
	if (!output)
		return (NULL);
	while (output[i])
	{
		if (output[i] == '$')
		{
			expand_initializer(&var);
			var->name_start = i + 1;
			var->name_end = var->name_start;
			while (output[var->name_end] && output[var->name_end] > 32 &&\
			 output[var->name_end] != '$' && output[var->name_end] != 34 && output[var->name_end] != 39)
				var->name_end++;
			//var->name_end--;
			//printf("startname = %c\nendname = %c\nLen name = %d\n", output[var->name_start], output[var->name_end], var->name_end - var->name_start);
			//printf("index start : %d\nIndex end : %d\n", var->name_start, var->name_end);
			var->name_len = var->name_end - var->name_start;
			var->name = ft_substr(output, var->name_start, var->name_len);
			var->value = getenv(var->name);
			if (var->value)
				var->value_len = ft_strlen(var->value);
			rl = malloc(sizeof(char)* var->value_len + i + 1);
			if (!rl)
				printf("ERROR MALLOC EXPAND\n\n\n\n");
			i = 0;
			while (i < (var->name_start - 1))
			{
				rl[i] = output[i];
				i++;
			}
			p = 0;
			if(var->value)
			{
				while (var->value[p])
				{
					rl[i] = var->value[p];
					i++;
					p++;
				}
			}
			p = var->name_end;
			//printf("p = %d\nlen output = %ld", p, ft_strlen(output));
			while (output[p])
			{
				rl[i] = output[p];
				i++;
				p++;
			}
			rl[i] = '\0';
			output = ft_strdup(rl);
			//printf("LEN OUTPUT |%ld|\nLEN RL |%ld|", ft_strlen(output), ft_strlen(rl));
			//printf ("\033[31mrl : |%s|\033[0m\n]", rl);
			//printf ("\033[31moutput : |%s|\033[0m\n]", output);
			//printf("\033[38;5;220mVarName : %s\033[0m\n", var->name);
			//printf("\033[38;5;220mVarValue : %s\033[0m\n", var->value);
			i = 0;
			//free_expand(&var);
		}
		else
			i++;
	}
	return (free(rl), output);	
}