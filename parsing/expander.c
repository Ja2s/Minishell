/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gavairon <gavairon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:15:44 by jgavairo          #+#    #+#             */
/*   Updated: 2024/05/23 23:13:09 by gavairon         ###   ########.fr       */
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

int	doll_heredoc_helper(char **rl, int i)
{
	int	x;

	x = '$' * -1;
	if ((*rl)[i] == '$')
	{
		if (((*rl)[i + 1] == 34 || (*rl)[i + 1] == 39) && (*rl)[i - 1] != x)
		{
			ft_memmove(&(*rl)[i], &(*rl)[i + 1], strlen((*rl)) - i);
			i--;
		}
		else
			(*rl)[i] = (*rl)[i] * -1;
	}
	else
		i++;
	return (i);
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
				while ((*rl)[i])
					i = doll_heredoc_helper(rl, i);
			}
		}
		else
			i++;
	}
}

void	double_negativer(int i, char **rl)
{
	if ((*rl)[i] == ' ' || (*rl)[i] == '<' || \
	(*rl)[i] == '>' || (*rl)[i] == 39 || (*rl)[i] == '|')
		(*rl)[i] = (*rl)[i] * -1;
}

void	simple_negativer(int i, char **rl)
{
	if ((*rl)[i] == ' ' || (*rl)[i] == '$' || (*rl)[i] == '<' || \
	(*rl)[i] == '>' || (*rl)[i] == '|' || (*rl)[i] == 34)
		(*rl)[i] = (*rl)[i] * -1;
}

/*Cette fonction vas parcourir toute lentree, et passer en negatif les caracteres speciaux entre cotes, suivant ceux que nous voulons interpreter ou pas*/
void	negative_checker(char *rl)
{
	int		i;

	i = -1;
	doll_heredoc(&rl);
	while (rl[++i])
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
	}
}

int	expand_initializer(t_expand **var)
{
	(*var) = malloc(sizeof(t_expand));
	if (!*var)
		return (-1);
	(*var)->name_start = 0;
	(*var)->name_end = 0;
	(*var)->name_len = 0;
	(*var)->name = NULL;
	(*var)->value = NULL;
	(*var)->value_len = 0;
	(*var)->code_copy = 0;
	(*var)->nb_numbers = 0;
	(*var)->in_cote = -1;
	(*var)->in_redirecter = false;
	(*var)->output = NULL;
	(*var)->i = 0;
	(*var)->p = 0;
	(*var)->pos_doll = 0;
	return (0);
}

void	free_expand(t_expand **var)
{
	if ((*var)->name)
	{
		free((*var)->name);
		(*var)->name = NULL;
	}
	if ((*var)->value)
	{
		free((*var)->value);
		(*var)->value = NULL;
	}
	if (*var)
	{
		free(*var);
		(*var) = NULL;
	}
}

int	doll_echo(char *output, int i)
{
	if (output[i] == '$' && (ft_isspace(output[i + 1]) == \
	1 || ft_isalnum(output[i + 1], 1) == 0))
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
	return (free(output), tmp);
}

void	negative_checker_sp(char **rl)
{
	int		i;

	i = 0;
	doll_heredoc(&(*rl));
	while ((*rl)[i])
	{
		if (ft_isspace((*rl)[i]) == 1)
			(*rl)[i] = (*rl)[i] * -1;
		i++;
	}
}

void	in_cote_checker(t_expand **var, char *output, int i)
{
	while (i > 0)
	{
		if (output[i] == '"')
			(*var)->in_cote = (*var)->in_cote * -1;
		i--;
	}
}

void	in_redirection_checker(t_expand **var, char *output, int i)
{
	while (i >= 0)
	{
		if (output[i] == '>' || output[i] == '<')
			(*var)->in_redirecter = true;
		i--;
	}
}

int	space_in_value_checker(t_expand *var)
{
	int	i;

	i = 0;
	if (var->in_cote < 1 && var->in_redirecter == true)
	{
		while (var->value[i])
		{
			if (ft_isspace(var->value[i]) == 1)
				return (-1);
			i++;
		}
	}
	return (0);
}

int	exitcode_expander_init(t_expand **var, t_data **data, char **rl)
{
	if (!var)
		return (-1);
	(*var)->name_start = (*var)->i + 1;
	(*var)->name_end = (*var)->name_start + 1;
	(*var)->name_len = (*var)->name_end - (*var)->name_start;
	(*var)->name = ft_substr((*var)->output, \
	(*var)->name_start, (*var)->name_len);
	if (!(*var)->name)
		return (-1);
	(*var)->value = ft_itoa((*data)->exit_code);
	if ((*var)->value)
		(*var)->value_len = ft_strlen((*var)->value);
	*rl = ft_calloc(((*var)->value_len - (*var)->name_len + \
	ft_strlen((*var)->output) + 1), sizeof(char));
	if (!rl)
		return (-1);
	(*var)->i = 0;
	return (0);
}

int	exitcode_expander_finish(t_expand **var, char **rl)
{
	while ((*var)->output[(*var)->p])
	{
		(*rl)[(*var)->i] = (*var)->output[(*var)->p];
		(*var)->i++;
		(*var)->p++;
	}
	(*rl)[(*var)->i] = '\0';
	(*var)->output = ft_strdup(*rl);
	if (!(*var)->output)
		return (-1);
	(*var)->i = (*var)->pos_doll;
	return (0);
}

int	exitcode_expander(t_expand **var, t_data **data, char *rl)
{
	if (exitcode_expander_init(var, data, &rl) == -1)
		return (-1);
	while ((*var)->i < ((*var)->name_start - 1))
	{
		rl[(*var)->i] = (*var)->output[(*var)->i];
		(*var)->i++;
	}
	(*var)->p = 0;
	if ((*var)->value)
	{
		while ((*var)->value[(*var)->p])
		{
			rl[(*var)->i] = (*var)->value[(*var)->p];
			(*var)->i++;
			(*var)->p++;
		}
	}
	(*var)->p = (*var)->name_end;
	if (exitcode_expander_finish(var, &rl) == -1)
		return (-1);
}

void	end_name_search(t_expand **var)
{
	while ((*var)->output[(*var)->name_end] && \
	ft_isspace((*var)->output[(*var)->name_end]) == 0 && \
	((ft_isalnum((*var)->output[(*var)->name_end], 0) == 1) || \
	(*var)->output[(*var)->name_end] == '_'))
		(*var)->name_end++;
}

void	expand_on_output(t_expand **var, char **rl)
{
	(*var)->i = 0;
	while ((*var)->i < ((*var)->name_start - 1))
	{
		(*rl)[(*var)->i] = (*var)->output[(*var)->i];
		(*var)->i++;
	}
	if ((*var)->value && (*var)->in_cote > 0)
		negative_checker_sp(&(*var)->value);
	(*var)->p = 0;
	if ((*var)->value)
	{
		while ((*var)->value[(*var)->p])
		{
			(*rl)[(*var)->i] = (*var)->value[(*var)->p];
			(*var)->i++;
			(*var)->p++;
		}
	}
	(*var)->p = (*var)->name_end;
	while ((*var)->output[(*var)->p])
	{
		(*rl)[(*var)->i] = (*var)->output[(*var)->p];
		(*var)->i++;
		(*var)->p++;
	}
}

int	basic_expander_helper(t_expand **var, t_data *data, char **rl)
{
	in_cote_checker(&(*var), (*var)->output, (*var)->i);
	in_redirection_checker(&(*var), (*var)->output, (*var)->i);
	if (!(*var))
		return (-1);
	(*var)->name_start = (*var)->i + 1;
	(*var)->pos_doll = (*var)->i;
	(*var)->name_end = (*var)->name_start;
	end_name_search(var);
	(*var)->name_len = (*var)->name_end - (*var)->name_start;
	(*var)->name = ft_substr((*var)->output, (*var)->name_start, (*var)->name_len);
	if (!(*var)->name)
		return (-1);
	(*var)->value = ft_getenv((*var)->name, data->mini_env);
	if ((*var)->value)
	{
		(*var)->value_len = ft_strlen((*var)->value);
		if (space_in_value_checker((*var)) == -1)
			return (data->ambigous = 1, -1);
	}
	*rl = ft_calloc(((*var)->value_len - (*var)->name_len + ft_strlen((*var)->output) + 1), sizeof(char));
	if (!rl)
		return (-1);
	expand_on_output(var, rl);
	(*rl)[(*var)->i] = '\0';
	return (0);
}

int	basic_expander(char *rl, t_expand **var, t_data *data)
{
	if (basic_expander_helper(var, data, &rl) == -1)
		return (-1);
	(*var)->output = ft_strdup(rl);
	if ((*var)->value && rl[0])
		command_positiver((*var)->value);
	(*var)->i = ((*var)->pos_doll + (*var)->value_len - 1);
}

int	if_condition_expand(t_expand *var, int choice)
{
	if (choice == 1)
	{
		if (var->output[var->i] == '$' && var->output[var->i] && var->output[var->i] == 34 || var->output[var->i] == 39)
			return (1);
	}
	else if (choice == 2)
	{
		if (var->output[var->i] == '$' && (var->output[var->i + 1] == '\0' || \
		ft_isspace(var->output[var->i + 1]) == 1))
			return (1);
		if (var->output[var->i] == '$' && var->output[var->i + 1] == '$')
		{
			var->i++;
			return (1);
		}
	}
	return (0);
}

char	*dolls_expander(char *rl, t_env *mini_env, t_data *data)
{
	t_expand	*var;

	if (expand_initializer(&var) == -1)
		return (NULL);
	if (rl)
		var->output = ft_strdup(rl);
	if (!var->output)
		return (NULL);
	while (var->i < (int)ft_strlen(var->output))
	{
		var->value_len = 0;
		if (if_condition_expand(var, 1) == 1)
			var->output = del_doll(var->output, var->i);
		else if (if_condition_expand(var, 2) == 1)
			var->i++;
		else if (var->output[var->i] == '$' && var->output[var->i + 1] != '?')
			basic_expander(rl, &var, data);
		else if (var->output[var->i] == '$' && var->output[var->i + 1] == '?')
			exitcode_expander(&var, &data, rl);
		else
			var->i++;
	}
	return (free(rl), var->output);
}
