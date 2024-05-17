/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:15:44 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/17 16:28:19 by jgavairo         ###   ########.fr       */
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

char	*value_extractor(char *env)
{
	int		i;
	int		x;
	int		len;
	char	*str;

	len = 0;
	x = 0;
	i = 0;
	str = NULL;
	while (env[i] && env[i] != '=')
		i++;
	i++;
	x = i;
	while (env[i])
	{
		i++;
		len++;
	}
	str = ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (env[x])
		str[i++] = env[x++];
	return (str);
}

char	*name_extractor(char *env)
{
	int		i;
	char	*str;

	str = NULL;
	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	str = ft_calloc(i + 1, sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (env[i] && env[i] != '=')
	{
		str[i] = env[i];
		i++;
	}
	return (str);
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
		str = name_extractor(env);
	else if (choice == 2)
		str = value_extractor(env);
	if (str == NULL)
		return (NULL);
	return (str);
}

int	env_copyer(char **envp, t_env **mini_env)
{
	int		i;
	t_env	*tmp;

	i = 0;
	while (envp[i])
	{
		tmp = env_new();
		if (!tmp)
			return (-1);
		else
		{
			tmp->name = env_extractor(envp[i], 1);
			if (!tmp->name)
				return (-1);
			tmp->value = env_extractor(envp[i], 2);
			if (!tmp->value)
				return (-1);
			ft_envadd_back(mini_env, tmp);
			i++;
		}
	}
	return (0);
}

void	doll_heredoc(char **rl)
{
	int	i;
	
	i = 0;
	while ((*rl)[i])
	{
		if ((*rl)[i] == '<')
		{
			i++;
			if ((*rl)[i] == '<')
			{
				i++;
				while ((*rl)[i] == ' ' || (*rl)[i] == 39 || (*rl)[i] == 34)
					i++;
				if ((*rl)[i] == '$')
					(*rl)[i] = (*rl)[i] * -1;
			}
		}
		else
			i++;
	}
}

void	double_negativer(int i, char **rl)
{
	if ((*rl)[i] == ' ' || (*rl)[i] == '<' || (*rl)[i] == '>' || (*rl)[i] == 39 || (*rl)[i] == '|')
		(*rl)[i] = (*rl)[i] * -1;
}

void	simple_negativer(int i, char **rl)
{
	if ((*rl)[i] == ' ' || (*rl)[i] == '$' || (*rl)[i] == '<' || (*rl)[i] == '>' || (*rl)[i] == '|' || (*rl)[i] == 34)
		(*rl)[i] = (*rl)[i] * -1;
}

/*Cette fonction vas parcourir toute lentree, et passer en negatif les caracteres speciaux entre cotes, suivant ceux que nous voulons interpreter ou pas*/
void	negative_checker(char *rl)
{
	int		i;

	i = 0;
	doll_heredoc(&rl);
	while (rl[i])
	{
		if (rl[i] && rl[i] == 34)
		{
			i++;
			while (rl[i] && rl[i] != 34)
			{
				double_negativer(i, &rl);
				i++;
			}
		}
		if (rl[i] && rl[i] == 39)
		{
			i++;
			while (rl[i] && rl[i] != 39)
			{
 				simple_negativer(i, &rl);
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
	(*var)->code_copy = 0;
	(*var)->nb_numbers = 0;
	(*var)->in_cote = -1;
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

int	doll_echo(char *output, int i)
{
	if (output[i] == '$' && (ft_isspace(output[i + 1]) == 1 || ft_isalnum(output[i + 1], 1) == 0))
	{
		while (output[i] && output[i] != 'o')
			i--;
		if (output[i] == 'o')
		{
			i--;
			if (output[i] == 'h')
			{
				i--;
				if (output[i] == 'c')
				{
					i--;
					if (output[i] == 'e')
						return (1);
				}
			}
		}
	}
	return (0);
}

char	*del_doll(char *output, int i)
{
	int		x;
	char	*tmp;

	x = 0;
	tmp = ft_calloc(ft_strlen(output), sizeof (char));
	if (!tmp)
		return (NULL);
	while (x < i)
	{
		tmp[x] = output[x];
		x++;
	}
	i++;
	while (output[i])
	{
		tmp[x] = output[i];
		x++;
		i++;
	}
	return(free(output), tmp);
}

void	negative_checker_sp(char **rl)
{
	int		i;

	i = 0;
	doll_heredoc(&(*rl));
	while ((*rl)[i])
	{
		if(ft_isspace((*rl)[i]) == 1)
			(*rl)[i] = (*rl)[i] * -1;
		i++;
	}
}

void	in_cote_checker(t_expand **var, char *output, int i)
{
	int	x;

	x = i;
	while (output[x])
	{
		if (output[x] == '"')
			(*var)->in_cote = (*var)->in_cote * - 1;
		x--;
	}	
}

char *dolls_expander(char *rl, t_env *mini_env, t_data *data) 
{
	t_expand	*var;
	char		*output;
	int			p;
	int			i;
	int			pos_doll;
	
	output = NULL;
	i = 0;
	if (rl)
		output = ft_strdup(rl);
	if (!output)
		return (NULL);
	while (output[i])
	{
		if (output[i] == '$' && output[i + 1] && (output[i + 1] == 34 || output[i + 1] == 39))
			output = del_doll(output, i);
		else if (output[i] == '$' && doll_echo(output, i) == 1)
			i++;
		else if (output[i] == '$' && output[i + 1] != '?')
		{
			expand_initializer(&var);
			in_cote_checker(&var, output, i);
			if (!var)
				return (NULL);
			var->name_start = i + 1;
			pos_doll = i;
			var->name_end = var->name_start;
			while (output[var->name_end] && output[var->name_end] > 32 &&\
			 output[var->name_end] != '$' && output[var->name_end] != 34 && output[var->name_end] != 39 && output[var->name_end] != '\\' && ft_isalnum(output[var->name_end], 0) == 1 && ft_isdigit(output[var->name_end]) == 0)
				var->name_end++;
			if (ft_isdigit(output[var->name_end]) == 1)
				var->name_end++;
			var->name_len = var->name_end - var->name_start;
			var->name = ft_substr(output, var->name_start, var->name_len);
			if (!var->name)
				return (NULL);
			var->value = ft_getenv(var->name, mini_env);
			if (var->value)
				var->value_len = ft_strlen(var->value);
			rl = ft_calloc((var->value_len - var->name_len + ft_strlen(output) + 1), sizeof(char));
			if (!rl)
				return(NULL);
			i = 0;
			while (i < (var->name_start - 1))
			{
				rl[i] = output[i];
				i++;
			}
			if (var->in_cote > 0)
				negative_checker_sp(&var->value);
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
			while (output[p])
			{
				rl[i] = output[p];
				i++;
				p++;
			}
			rl[i] = '\0';
			output = ft_strdup(rl);
			i = pos_doll + var->value_len;
			command_positiver(var->value);
		}
		else if (output[i] == '$' && output[i + 1] == '?')
		{
			expand_initializer(&var);
			var->code_copy = data->exit_code;
			var->nb_numbers = 0;
			if (!var)
				return (NULL);
			var->name_start = i + 1;
			var->name_end = var->name_start + 1;
			var->name_len = var->name_end - var->name_start;
			var->name = ft_substr(output, var->name_start, var->name_len);
			if (!var->name)
				return (NULL);
			var->value = ft_itoa(data->exit_code);
			if (var->value)
				var->value_len = ft_strlen(var->value);
			rl = ft_calloc((var->value_len - var->name_len + ft_strlen(output) + 1), sizeof(char));
			if (!rl)
				return(NULL);
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
			while (output[p])
			{
				rl[i] = output[p];
				i++;
				p++;
			}
			rl[i] = '\0';
			output = ft_strdup(rl);
			i = 0;	
		}
		else
			i++;
	}
	return (free(rl), output);	
}
