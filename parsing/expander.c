/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:15:44 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/02 16:05:41 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*ft_envlast(t_env *lst)
{
	t_env	*tmp;

	if (!lst)
		return (NULL);
	tmp = lst;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	ft_envadd_back(t_env **env, t_env *new)
{
	if (*env)
		ft_envlast(*env)->next = new;
	else
		*env = new;
}

t_env	*env_new(void)
{
	t_env	*new_elem;

	new_elem = malloc(sizeof(t_env));
	if (!new_elem)
		return (NULL);
	new_elem->name = NULL;
	new_elem->next = NULL;
	new_elem->value = NULL;
	return (new_elem);
}

char	*env_extractor(char	*env, int choice)
{
	int		i;
	int		x;
	int		len;
	char	*str;

	len = 0;
	x = 0;
	i = 0;
	str = NULL;
	if (choice == 1)
	{
		while(env[i] && env[i] != '=')
			i++;
		str = ft_calloc(i, sizeof(char));
		i = 0; 
		while(env[i] && env[i] != '=')
		{
			str[i] = env[i]; 
			i++;
		}
	}
	else if (choice == 2)
	{
		while (env[i] && env[i] != '=')
			i++;
		i++;
		x = i;
		while (env[i])
		{
			i++;
			len++;
		}
		str = ft_calloc(len, sizeof(char));
		i = 0;
		while (env[x])
			str[i++] = env[x++];
	}
	return (str);
}

int	env_copyer(char **envp, t_env **mini_env)
{
	int		i;
	t_env	*tmp;

	i = 0;
	while(envp[i])
	{
		if((tmp = env_new()))
		{
			tmp->name = env_extractor(envp[i], 1);
			tmp->value = env_extractor(envp[i], 2);
			ft_envadd_back(mini_env, tmp);
			i++;
		}
		else
			return (-1);
	}
	return (0);
}

void	doll_heredoc(char **rl)
{
	int	i;
	
	i = 0;
	while((*rl)[i])
	{
		if ((*rl)[i] == '<')
		{
			i++;
			if ((*rl)[i] == '<')
			{
				i++;
				while ((*rl)[i] == ' ')
					i++;
				if ((*rl)[i] == '$')
					(*rl)[i] = (*rl)[i] * -1;
			}
		}
		else
			i++;
	}
}

/*Cette fonction vas parcourir toute lentree, et passer en negatif les caracteres speciaux entre cotes, suivant ceux que nous voulons interpreter ou pas*/
void	negative_checker(char *rl)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(rl);
	doll_heredoc(&rl);
	while (i < len)
	{
		if (rl[i] == 34)
		{
			i++;
			while (rl[i] != 34)
			{
				if (rl[i] == ' ' || rl[i] == '<' || rl[i] == '>' || rl[i] == 39)
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

char *dolls_expander(char *rl, t_env *mini_env) 
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
			//printf("startname = %c\nendname = %c\nLen name = %d\n", output[var->name_start], output[var->name_end], var->name_end - var->name_start);
			//printf("index start : %d\nIndex end : %d\n", var->name_start, var->name_end);
			var->name_len = var->name_end - var->name_start;
			var->name = ft_substr(output, var->name_start, var->name_len);
			var->value = ft_getenv(var->name, mini_env);
			if (var->value)
				var->value_len = ft_strlen(var->value);
			rl = malloc(sizeof(char)* var->value_len + i + 2);
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
		}
		else
			i++;
	}
	return (free(rl), output);	
}